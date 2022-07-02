
#include "Camera.hpp"

#include <cmath>

#include "glm/glm.hpp"
#include "MathUtils.hpp"

namespace WoohooRT
{
  Camera::Camera(const Vec3& pos, const Vec3& target, const Vec3& up, float vFov, float aspectRatio)
  {
    m_position = pos;

    Vec3 w = UnitVector(pos - target);
    Vec3 u = UnitVector(glm::cross(up, w));
    Vec3 v = glm::cross(w, u);

    float theta = glm::radians(vFov);
    float h = std::tan(theta / 2.0f);
    m_viewportHeight = 2.0f * h;
    m_viewportWidth = aspectRatio * m_viewportHeight;
    m_aspectRatio = aspectRatio;

    m_right = m_viewportWidth * u;
    m_up = m_viewportHeight * v;
    m_lowerLeftCorner = m_position - m_right / 2.0f - m_up / 2.0f - w;
  }

} // namespace WoohooRT
