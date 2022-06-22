
#include <limits>
#include <random>

#include "glm/gtc/epsilon.hpp"
#include "Types.hpp"

namespace WoohooRT
{
  const float FLOAT_INFINITY = std::numeric_limits<float>::infinity();
  const float PI = 3.1415926535897932385f;

  /* Vector operations */

  inline Vec3 UnitVector(const Vec3& vector)
  {
    return glm::normalize(vector);
  }

  inline float SquaredLength(Vec3 vector)
  {
    return vector.x * vector.x + vector.y * vector.y + vector.z * vector.z;
  }

  inline float RandomFloat()
  {
    static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
    static std::mt19937 generator;
    return distribution(generator);
  }

  inline float RandomFloat(float min, float max)
  {
    static std::uniform_real_distribution<float> distribution(min, max);
    static std::mt19937 generator;
    return distribution(generator);
  }

  inline bool Vec3EpsilonEqual(float epsilon, const Vec3& vec1, const Vec3& vec2)
  {
    return glm::all(glm::epsilonEqual(vec1, vec2, epsilon));
  }

  inline bool Vec3NearZero(const Vec3& vec)
  {
    static float min = 1e-8f;
    return (fabs(vec.x) < min && fabs(vec.y) < min && fabs(vec.z) < min);
  }

  /* Randomization operations */

  inline Vec3 RandomVec3()
  {
    return Vec3(RandomFloat(), RandomFloat(), RandomFloat());
  }

  inline Vec3 RandomUnitVec3()
  {
    return UnitVector(Vec3(RandomFloat(), RandomFloat(), RandomFloat()));
  }

  inline Vec3 RandomVec3(float min, float max)
  {
    return Vec3(RandomFloat(min, max), RandomFloat(min, max), RandomFloat(min, max));
  }

  inline Vec3 RandomVec3InUnitSphere()
  {
    while (true)
    {
      Vec3 p = RandomVec3(-1.0f, 1.0f);
      if (SquaredLength(p) >= 1.0f)
      {
        continue;
      }
      return p;
    }
  }

  inline Vec3 RandomUnitVec3InUnitSphere()
  {
    while (true)
    {
      Vec3 p = RandomVec3(-1.0f, 1.0f);
      if (SquaredLength(p) >= 1.0f)
      {
        continue;
      }
      return UnitVector(p);
    }
  }

  inline Vec3 RandomUnitVec3InRandomUnitSphere()
  {
    return UnitVector(RandomVec3InUnitSphere());
  }

  inline Vec3 RandomInHemisphere(const Vec3& normal)
  {
    Vec3 vectorInUnitSphere = RandomVec3InUnitSphere();
    if (glm::dot(vectorInUnitSphere, normal) > 0.0f) // In the same hemisphere as the normal
    {
      return vectorInUnitSphere;
    }
    else
    {
      return -vectorInUnitSphere;
    }
  }

  /* 3D Math Operations */

  inline Vec3 Reflect(const Vec3& v1, const Vec3& v2)
  {
    return v1 - 2.0f * glm::dot(v1, v2) * v2;
  }

} // namespace WoohooRT
