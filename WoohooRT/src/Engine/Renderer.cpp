
#include "Renderer.hpp"

#include <cmath>
#include <iostream>

#include "glm/glm.hpp"

#include "MathUtils.hpp"
#include "../Geometry/Sphere.hpp"
#include "../Material/LambertianMaterial.hpp"
#include "../Material/MetalMaterial.hpp"
#include "../Material/DielectricMaterial.hpp"

namespace WoohooRT
{
  CPURenderer::CPURenderer()
  {
    float aspect = 16.0f / 9.0f;
    int imageWidth = 400;
    int imageHeight = static_cast<int>(imageWidth / aspect);

    m_samplesPerPixel = 100;
    m_maxBounce = 10;

    // Allocate output buffer
    m_outputBuffer = new int[3 * imageWidth * imageHeight];
    m_outputBufferLength = 3 * imageWidth * imageHeight;
    m_outputBufferOffset = 0;

    // Camera
    Vec3 eye = Vec3(3.0, 3.0f, 2.0f);
    Vec3 target = Vec3(-1.0f, 0.0f, -1.0f);
    Vec3 up = Vec3(0.0f, 1.0f, 0.0f);
    float distToFocus = glm::length(eye - target);
    float aperture = 2.0f;
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
    auto lambertianMat1 = std::make_shared<LambertianMaterial>(Vec3(0.8f, 0.8f, 0.0f));
    auto lambertianMat2 = std::make_shared<LambertianMaterial>(Vec3(0.1f, 0.2f, 0.5f));
    auto DielectricMat = std::make_shared<DielectricMaterial>(Vec3(1.0f, 1.0f, 1.0f), 1.5f);
    auto MetalMat = std::make_shared<MetalMaterial>(Vec3(0.8f, 0.6f, 0.2f), 0.0f);

    m_scene = std::shared_ptr<Scene>(new Scene());

    float R = std::cos(PI / 4.0f);
    m_scene->AddGeometry(std::shared_ptr<Sphere>(new Sphere(Vec3(0.0f, -100.5f, -1.0f), 100.0f, lambertianMat1)));
    m_scene->AddGeometry(std::shared_ptr<Sphere>(new Sphere(Vec3(0.0f, 0.0f, -1.0f), 0.5f, lambertianMat2)));

    // These two are same sphere. One for inside, one for outside
    m_scene->AddGeometry(std::shared_ptr<Sphere>(new Sphere(Vec3(-1.0f, 0.0f, -1.0f), 0.5f, DielectricMat)));
    m_scene->AddGeometry(std::shared_ptr<Sphere>(new Sphere(Vec3(-1.0f, 0.0f, -1.0f), -0.45f, DielectricMat)));

    m_scene->AddGeometry(std::shared_ptr<Sphere>(new Sphere(Vec3(1.0f, 0.0f, -1.0f), 0.45f, MetalMat)));
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
