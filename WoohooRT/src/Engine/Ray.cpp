
#include "Ray.hpp"

#include "MathUtils.hpp"

namespace WoohooRT
{
  Ray::Ray(Vec3 origin, Vec3 direction)
  {
    m_origin = origin;
    m_direction = UnitVector(direction);
  }

} // namespace WoohooRT
