
#include "Renderer.hpp"

#include <iostream>

#include "MathUtils.hpp"
#include "../Geometry/Sphere.hpp"

namespace WoohooRT
{
  float aspect = 16.0f / 9.0f;

  CPURenderer::CPURenderer()
  {
    // Camera
    m_camera = std::shared_ptr<Camera>
    (
      new Camera
      (
        Vec3(0.0f),
        2.0f * aspect,
        2.0f,
        1.0f
      )
    );

    // Image
    m_image = std::shared_ptr<Image>(new Image(400, static_cast<int>(400 / aspect)));

    // Scene
    m_scene = std::shared_ptr<Scene>(new Scene());
    m_scene->AddGeometry(std::shared_ptr<Sphere>(new Sphere(Vec3(0.0f, 0.0f, -1.0f), 0.5f)));
    m_scene->AddGeometry(std::shared_ptr<Sphere>(new Sphere(Vec3(0.0f, -100.5f, -1.0f), 100.0f)));
  }

  void CPURenderer::Render()
  {
    std::cout << "P3\n" << m_image->m_width << ' ' << m_image->m_height << "\n255\n";

    for (int j = m_image->m_height - 1; j >= 0; --j)
    {
      std::cerr << "\rScanlines remaining: " << j << std::flush;

      for (int i = 0; i < m_image->m_width; ++i)
      {
        float u = float(i) / (m_image->m_width - 1);
        float v = float(j) / (m_image->m_height - 1);
        
        Ray r
        (
          m_camera->m_position,
          m_camera->m_lowerLeftCorner + u * m_camera->m_right + v * m_camera->m_up - m_camera->m_position
        ); // TODO Optimize out

        Vec3 pixelColor = RayColor(r);

        WriteColor(pixelColor); // TODO optimize out
      }
    }

    std::cerr << "\nDone.\n";
  }

  Vec3 CPURenderer::RayColor(const Ray& ray)
  {
    Intersection intersection; // TODO optimize out

    // Scene
    if (m_scene->Hit(ray, 0, FLOAT_INFINITY, intersection))
    {
      return 0.5f * (intersection.normal + Vec3(1.0f));
    }

    // Background
    float t = 0.5f * (ray.m_direction.y + 1.0f);
    return Vec3(1.0f - t) + t * Vec3(0.5f, 0.7f, 1.0f); // TODO optimize out
  }

  void CPURenderer::WriteColor(const Vec3& color)
  {
    std::cout << static_cast<int>(255.999f * color.x)
    << ' ' << static_cast<int>(255.999f * color.y)
    << ' ' << static_cast<int>(255.999f * color.z) << '\n';
  }

} // namespace WoohooRT
