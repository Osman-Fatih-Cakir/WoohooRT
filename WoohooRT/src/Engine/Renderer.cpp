
#include "Renderer.hpp"

#include <cmath>
#include <iostream>
#include <thread>
#include <vector>

#include "glm/glm.hpp"

#include "MathUtils.hpp"
#include "../Geometry/Sphere.hpp"
#include "../Material/Material.hpp"
#include "../Material/LambertianMaterial.hpp"
#include "../Material/MetalMaterial.hpp"
#include "../Material/DielectricMaterial.hpp"

namespace WoohooRT
{
  CPURenderer::CPURenderer()
  {
    std::cerr << "Initalizing...\n";

    float aspect = 3.0f / 2.0f;
    int imageWidth = 1200;
    int imageHeight = static_cast<int>(imageWidth / aspect);

    m_samplesPerPixel = 500;
    m_maxBounce = 50;

    m_numChannels = 3;

    // Allocate output buffer
    m_outputBufferLength = m_numChannels * imageWidth * imageHeight;
    m_outputBuffer = new int[m_outputBufferLength];

    // Camera
    Vec3 eye = Vec3(13.0, 2.0f, 3.0f);
    Vec3 target = Vec3(0.0f, 0.0f, 0.0f);
    Vec3 up = Vec3(0.0f, 1.0f, 0.0f);
    float distToFocus = 10.0f;
    float aperture = 0.1f;
    m_camera = std::shared_ptr<Camera>
    (
      new Camera
      (
        eye,
        target,
        up,
        20.0f,
        aspect,
        aperture,
        distToFocus
      )
    );

    // Image
    m_image = std::shared_ptr<Image>(new Image(imageWidth, imageHeight));

    // Scene
    m_scene = std::shared_ptr<Scene>(new Scene());
    CreateRandomScene();
  }

  CPURenderer::~CPURenderer()
  {
    delete[] m_outputBuffer;
  }

  void CPURenderer::BeginRender()
  {
    std::cerr << "Initalizing is complete.\n";

    unsigned int hardwareConcurrencyMax = std::thread::hardware_concurrency();
    if (hardwareConcurrencyMax == 0)
    {
      std::cerr << "Error when getting concurrency count, assuming 1 cores.\n";
      hardwareConcurrencyMax = 1;
    }
    unsigned int threadCount = hardwareConcurrencyMax;

    // Create thread data
    std::vector<std::thread> threads;
    threads.reserve(threadCount);
    std::vector<struct ThreadData> tds;
    tds.resize(threadCount);

    float count = 0;
    for (unsigned int i = 0; i < threadCount; i++)
    {
      tds[i].scene = m_scene;
      tds[i].size.x = m_image->m_width;
      tds[i].size.y = m_image->m_height;
      tds[i].index = i;
      tds[i].buffer = m_outputBuffer;
      tds[i].startX = static_cast<int>(std::floor(i * m_image->m_width / threadCount));
      tds[i].endX = static_cast<int>(std::ceil((i + 1) * m_image->m_width / threadCount));
      tds[i].samplesPerPixel = m_samplesPerPixel;
      tds[i].numChannels = m_numChannels;
    }

    // Write to .ppm file output
    std::cout << "P3\n" << m_image->m_width << ' ' << m_image->m_height << "\n255\n";

    // Start rendering
    std::cerr << "Starting rendering...\n";
    auto start = std::chrono::steady_clock::now();
    for (unsigned int i = 0; i < threadCount; i++)
    {
      threads.emplace_back(&CPURenderer::RenderThread, this, &tds[i]);
    }
    for (auto it = threads.begin(); it != threads.end(); ++it)
    {
      it->join();
    }
    auto end = std::chrono::steady_clock::now();
    // Rendering is done

    std::cerr << "\nRendering is done. Writing image to file...\n";

    WriteColor();

    std::cerr << "Rendering time: " << std::chrono::duration_cast<std::chrono::seconds>(end - start).count() << " seconds.\n\n";
  }

  void CPURenderer::RenderThread(ThreadData* td)
  {
    for (int j = 0; j < td->size.y; ++j)
    {
      std::cerr << "\r||" << j << "||" << std::flush; // TODO delete
      for (int i = td->startX; i < td->endX; ++i)
      {
        Vec3 pixelColor = Vec3(0.0f);
        for (int s = 0; s < m_samplesPerPixel; ++s)
        {
          float u = (i + RandomFloat()) / (td->size.x - 1);
          float v = (j + RandomFloat()) / (td->size.y - 1);

          Ray ray = m_camera->GetRay(u, v);

          pixelColor += RayColor(td->scene, ray, m_maxBounce);
        }

        SaveColor(td, pixelColor, td->size.y - 1 - j, i); // Flip height
      }
    }
  }

  void CPURenderer::CreateRandomScene()
  {
    auto groundMat = std::make_shared<LambertianMaterial>(Vec3(0.5f, 0.5f, 0.5f));
    m_scene->AddGeometry(std::shared_ptr<Sphere>(new Sphere(Vec3(0.0f, -1000.f, 0.0f), 1000.0f, groundMat)));

    for (int a = -11; a < 11; a++)
    {
      for (int b = -11; b < 11; b++)
      {
        float chooseMat = RandomFloat();
        Vec3 center = Vec3(a + 0.9f * RandomFloat(), 0.2, b + 0.9f * RandomFloat());

        if (glm::length(center - Vec3(4.0f, 0.2f, 0.0f)) > 0.9f)
        {
          std::shared_ptr<Material> sphereMat;

          if (chooseMat < 0.8f)
          {
            // Diffuse
            Vec3 albedo = RandomVec3() * RandomVec3();
            sphereMat = std::make_shared<LambertianMaterial>(albedo);
            m_scene->AddGeometry(std::shared_ptr<Sphere>(new Sphere(center, 0.2f, sphereMat)));
          }
          else if (chooseMat < 0.95f)
          {
            // Metal
            Vec3 albedo = RandomVec3(0.5f, 1.0f);
            float fuzz = RandomFloat(0.0f, 0.5f);
            sphereMat = std::make_shared<MetalMaterial>(albedo, fuzz);;
            m_scene->AddGeometry(std::shared_ptr<Sphere>(new Sphere(center, 0.2f, sphereMat)));
          }
          else
          {
            // Glass
            sphereMat = std::make_shared<DielectricMaterial>(Vec3(1.0f), 1.5f);
            m_scene->AddGeometry(std::shared_ptr<Sphere>(new Sphere(center, 0.2f, sphereMat)));
          }
        }
      }
    }

    auto mat1 = std::make_shared<DielectricMaterial>(Vec3(1.0f), 1.5f);
    m_scene->AddGeometry(std::shared_ptr<Sphere>(new Sphere(Vec3(0.0f, 1.0f, 0.0f), 1.0f, mat1)));

    auto mat2 = std::make_shared<LambertianMaterial>(Vec3(0.4f, 0.2f, 0.1f));
    m_scene->AddGeometry(std::shared_ptr<Sphere>(new Sphere(Vec3(-4.0f, 1.0f, 0.0f), 1.0f, mat2)));

    auto mat3 = std::make_shared<MetalMaterial>(Vec3(0.7f, 0.6f, 0.5f), 0.0f);
    m_scene->AddGeometry(std::shared_ptr<Sphere>(new Sphere(Vec3(4.0f, 1.0f, 0.0f), 1.0f, mat3)));
  }

  Vec3 CPURenderer::RayColor(std::shared_ptr<Scene> scene, const Ray& ray, int depth)
  {
    if (depth <= 0)
    {
      return Vec3(0.0f);
    }

    Intersection intersection;

    // Scene
    if (scene->Hit(ray, 0.001f, FLOAT_INFINITY, intersection))
    {
      Ray scattered;
      Vec3 attenuation;
      if (intersection.material->Scatter(ray, intersection, attenuation, scattered))
      {
        return attenuation * RayColor(scene, scattered, depth - 1);
      }

      return Vec3(0.0f);
    }

    // Background
    float t = 0.5f * (ray.m_direction.y + 1.0f);
    return Vec3(1.0f - t) + t * Vec3(0.5f, 0.7f, 1.0f);
  }

  void CPURenderer::SaveColor(struct ThreadData* td, const Vec3& color, int col, int row)
  {
    float r = color.x;
    float g = color.y;
    float b = color.z;

    float scale = 1.0f / td->samplesPerPixel;
    r = glm::sqrt(scale * r);
    g = glm::sqrt(scale * g);
    b = glm::sqrt(scale * b);

    unsigned int index = td->numChannels * (td->size.x * col + row);

    td->buffer[index] = static_cast<int>(256 * glm::clamp(r, 0.0f, 0.999f));
    td->buffer[index + 1] = static_cast<int>(256 * glm::clamp(g, 0.0f, 0.999f));
    td->buffer[index + 2] = static_cast<int>(256 * glm::clamp(b, 0.0f, 0.999f));
  }

  void CPURenderer::WriteColor()
  {
    for (unsigned int i = 0; i < m_outputBufferLength; i+=3)
    {
      if (i % m_image->m_width == 0 && m_image->m_width != 0)
      {
        std::cout << "\n";
      }
      std::cout << m_outputBuffer[i + 0] << " "
                << m_outputBuffer[i + 1] << " "
                << m_outputBuffer[i + 2] << " ";
    }
  }
} // namespace WoohooRT
