#pragma once

#include <memory>
#include <vector>

#include "Types.hpp"
#include "Camera.hpp"
#include "Ray.hpp"
#include "Image.hpp"
#include "Scene.hpp"
#include "../Geometry/Geometry.hpp"

namespace WoohooRT
{
  class Renderer
  {
  public:
    virtual void Render() = 0;
    
  };

  class CPURenderer : public Renderer
  {
  public:
    CPURenderer();
    void Render();

  private:
    Vec3 RayColor(const Ray& ray);
    void WriteColor(const Vec3& color);

  public:
    std::shared_ptr<Camera> m_camera;
    std::shared_ptr<Image> m_image;
    std::shared_ptr<Scene> m_scene;

  private:
  };

} // namespace WoohooRT
