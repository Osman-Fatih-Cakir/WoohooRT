#pragma once

#include "Types.hpp"

namespace WoohooRT
{
  class Camera
  {
  public:
    Camera(Vec3 pos, float viewportWidth, float viewportHeight, float focalLength);

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
