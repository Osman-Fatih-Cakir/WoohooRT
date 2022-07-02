#pragma once

#include "Types.hpp"
#include "Ray.hpp"

namespace WoohooRT
{
  class Camera
  {
  public:
    Camera(const Vec3& pos, const Vec3& target, const Vec3& up, float vFov, float aspectRatio);

    inline Ray GetRay(float u, float v) const
    {
      /* TODO Maybe returning a pointer and the renderer (or whoever calls) have a preallocated ray array */
      return Ray(m_position, m_lowerLeftCorner + u * m_right + v * m_up - m_position);
    }

    float m_viewportHeight;
    float m_viewportWidth;
    float m_aspectRatio;

    Vec3 m_position;
    Vec3 m_right;
    Vec3 m_up;
    Vec3 m_lowerLeftCorner;
  };

} // namespace WoohooRT
