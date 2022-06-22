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
    ~CPURenderer();
    void Render();

  private:
    Vec3 RayColor(const Ray& ray, int depth);
    void SaveColor(const Vec3& color);
    void WriteColor();

  public:
    std::shared_ptr<Camera> m_camera;
    std::shared_ptr<Image> m_image;
    std::shared_ptr<Scene> m_scene;

    int m_samplesPerPixel;
    int m_maxBounce;

  private:
    int* m_outputBuffer;
    unsigned int m_outputBufferLength;
    unsigned int m_outputBufferOffset;
  };

} // namespace WoohooRT
