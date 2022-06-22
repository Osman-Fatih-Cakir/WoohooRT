
#include "Ray.hpp"

#include "MathUtils.hpp"

namespace WoohooRT
{
  Ray::Ray()
  {
    m_origin = Vec3(0.0f);
    m_direction = Vec3(0.0f, 0.0f, -1.0f);
  }

  Ray::Ray(Vec3 origin, Vec3 direction)
  {
    m_origin = origin;
    m_direction = UnitVector(direction);
  }

} // namespace WoohooRT
