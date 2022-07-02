
#include "Renderer.hpp"

#include <cmath>
#include <iostream>

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
    std::cerr << "Initalizing scene...\n";

    float aspect = 3.0f / 2.0f;
    int imageWidth = 600;
    int imageHeight = static_cast<int>(imageWidth / aspect);

    m_samplesPerPixel = 100;
    m_maxBounce = 50;

    // Allocate output buffer
    m_outputBuffer = new int[3 * imageWidth * imageHeight];
    m_outputBufferLength = 3 * imageWidth * imageHeight;
    m_outputBufferOffset = 0;

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
    std::cerr << "Initalizing scene is complete. Starting rendering...\n";
  }

  CPURenderer::~CPURenderer()
  {
    delete[] m_outputBuffer;
  }

  void CPURenderer::Render()
  {
    std::cout << "P3\n" << m_image->m_width << ' ' << m_image->m_height << "\n255\n";

    for (int j = m_image->m_height - 1; j >= 0; --j)
    {
      std::cerr << "\rScanlines remaining: " << j << std::flush;

      for (int i = 0; i < m_image->m_width; ++i)
      {
        Vec3 pixelColor = Vec3(0.0f);
        for (int s = 0; s < m_samplesPerPixel; ++s)
        {
          float u = (i + RandomFloat()) / (m_image->m_width - 1);
          float v = (j + RandomFloat()) / (m_image->m_height - 1);

          Ray ray = m_camera->GetRay(u, v);

          pixelColor += RayColor(ray, m_maxBounce);
        }

        SaveColor(pixelColor);
      }
    }

    std::cerr << "\nWriting image to file...\n";

    WriteColor();

    std::cerr << "\nDone.\n";
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

  Vec3 CPURenderer::RayColor(const Ray& ray, int depth)
  {
    if (depth <= 0)
    {
      return Vec3(0.0f);
    }

    Intersection intersection;

    // Scene
    if (m_scene->Hit(ray, 0.001f, FLOAT_INFINITY, intersection))
    {
      Ray scattered;
      Vec3 attenuation;
      if (intersection.material->Scatter(ray, intersection, attenuation, scattered))
      {
        return attenuation * RayColor(scattered, depth - 1);
      }

      return Vec3(0.0f);
    }

    // Background
    float t = 0.5f * (ray.m_direction.y + 1.0f);
    return Vec3(1.0f - t) + t * Vec3(0.5f, 0.7f, 1.0f);
  }

  void CPURenderer::SaveColor(const Vec3& color)
  {
    float r = color.x;
    float g = color.y;
    float b = color.z;

    float scale = 1.0f / m_samplesPerPixel;
    r = glm::sqrt(scale * r);
    g = glm::sqrt(scale * g);
    b = glm::sqrt(scale * b);

    m_outputBuffer[m_outputBufferOffset++] = static_cast<int>(256 * glm::clamp(r, 0.0f, 0.999f));
    m_outputBuffer[m_outputBufferOffset++] = static_cast<int>(256 * glm::clamp(g, 0.0f, 0.999f));
    m_outputBuffer[m_outputBufferOffset++] = static_cast<int>(256 * glm::clamp(b, 0.0f, 0.999f));
  }

  void CPURenderer::WriteColor()
  {
    for (unsigned int i = 0; i < m_outputBufferLength; i+=3)
    {
      std::cout << m_outputBuffer[i + 0] << " "
                << m_outputBuffer[i + 1] << " "
                << m_outputBuffer[i + 2] << std::endl;
    }
  }
} // namespace WoohooRT
