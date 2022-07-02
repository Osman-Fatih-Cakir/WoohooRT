#pragma once

#include "Types.hpp"
#include "Ray.hpp"
#include "MathUtils.hpp"

namespace WoohooRT
{
  class Camera
  {
  public:
    Camera(const Vec3& pos, const Vec3& target, const Vec3& up, float vFov, float aspectRatio, float aperture, float focusDist);

    inline Ray GetRay(float s, float t) const
    {
      /* TODO Maybe returning a pointer and the renderer (or whoever calls) have a preallocated ray array */
      Vec3 rd = m_lensRadius * RandomInUnitDisk();
      Vec3 offset = m_u * rd.x + m_v * rd.y;
      return Ray(m_position + offset, m_lowerLeftCorner + s * m_horizontal + t * m_vertical - m_position - offset);
    }

    float m_viewportHeight;
    float m_viewportWidth;
    float m_aspectRatio;

    Vec3 m_position;
    Vec3 m_horizontal;
    Vec3 m_vertical;
    Vec3 m_lowerLeftCorner;

    Vec3 m_w; // Vector points camera back
    Vec3 m_v; // Vector points camera up
    Vec3 m_u; // Vector points camera right
    float m_lensRadius;
  };

} // namespace WoohooRT
