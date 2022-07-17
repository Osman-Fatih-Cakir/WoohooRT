
#include "Camera.hpp"

#include <cmath>

#include "glm/glm.hpp"

namespace WoohooRT
{
  Camera::Camera(const Vec3& pos, const Vec3& target, const Vec3& up, float vFov, float aspectRatio, float aperture, float focusDist)
  {
    m_position = pos;

    m_w = UnitVector(pos - target);
    m_u = UnitVector(glm::cross(up, m_w));
    m_v = glm::cross(m_w, m_u);

    float theta = glm::radians(vFov);
    float h = std::tan(theta / 2.0f);
    m_viewportHeight = 2.0f * h;
    m_viewportWidth = aspectRatio * m_viewportHeight;
    m_aspectRatio = aspectRatio;

    m_horizontal = focusDist * m_viewportWidth * m_u;
    m_vertical = focusDist * m_viewportHeight * m_v;
    m_lowerLeftCorner = m_position - m_horizontal / 2.0f - m_vertical / 2.0f - focusDist * m_w;

    m_lensRadius = aperture / 2.0f;
  }

} // namespace WoohooRT
