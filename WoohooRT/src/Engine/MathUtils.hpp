
#include <limits>

#include "Types.hpp"

namespace WoohooRT
{
  const float FLOAT_INFINITY = std::numeric_limits<float>::infinity();
  const float PI = 3.1415926535897932385f;

  inline Vec3 UnitVector(const Vec3& vector)
  {
    return glm::normalize(vector);
  }

  inline float SquaredLength(Vec3 vector)
  {
    return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;
  }

} // namespace WoohooRT
