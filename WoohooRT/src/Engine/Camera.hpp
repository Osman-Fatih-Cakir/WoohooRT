#pragma once

#include "Types.hpp"
#include "Ray.hpp"

namespace WoohooRT
{
  class Camera
  {
  public:
    Camera(Vec3 pos, float viewportWidth, float viewportHeight, float focalLength);

    inline Ray GetRay(float u, float v) const
    {
      /* TODO Maybe returning a pointer and the renderer (or whoever calls) have a preallocated ray array */
      return Ray(m_position, m_lowerLeftCorner + u * m_right + v * m_up - m_position);
    }

    float m_viewportHeight;
    float m_viewportWidth;
    float m_aspectRatio;
    float m_focalLength;

    Vec3 m_position;
    Vec3 m_right;
    Vec3 m_up;
    Vec3 m_lowerLeftCorner;
  };

} // namespace WoohooRT
