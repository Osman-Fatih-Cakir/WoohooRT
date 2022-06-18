
#include "Camera.hpp"
#include "MathUtils.hpp"

namespace WoohooRT
{
  Camera::Camera(Vec3 pos, float viewportWidth, float viewportHeight, float focalLength)
  {
    m_position = pos;
    m_right = Vec3(viewportWidth, 0.0f, 0.0f);
    m_up = Vec3(0.0f, viewportHeight, 0.0f);
    m_viewportWidth = viewportWidth;
    m_viewportHeight = viewportHeight;
    m_aspectRatio = viewportWidth / viewportHeight;
    m_focalLength = focalLength;
    m_lowerLeftCorner = m_position - m_right / 2.0f - m_up / 2.0f - Vec3(0.0f, 0.0f, focalLength);
  }

} // namespace WoohooRT
