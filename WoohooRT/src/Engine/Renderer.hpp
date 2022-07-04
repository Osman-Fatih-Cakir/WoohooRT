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
  struct ThreadData
  {
    std::shared_ptr<Scene> scene;
    IVec2 size;
    int index;
    int* buffer;
    int startX;
    int endX;
    int samplesPerPixel;
    int numChannels;
  };

  class Renderer
  {
  public:
    virtual void BeginRender() = 0;
  };

  class CPURenderer : public Renderer
  {
  public:
    CPURenderer();
    ~CPURenderer();
    void BeginRender();

  private:
    void CreateRandomScene();
    void RenderThread(struct ThreadData* td);
    Vec3 RayColor(std::shared_ptr<Scene> scene, const Ray& ray, int depth);
    void SaveColor(struct ThreadData* td, const Vec3& color, int col, int row);
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
    int m_numChannels;
  };

} // namespace WoohooRT
