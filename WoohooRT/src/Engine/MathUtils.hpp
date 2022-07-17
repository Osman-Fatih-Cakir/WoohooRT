#pragma once

#include <limits>
#include <random>
#include <cmath>

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

  inline float SquaredLength(const Vec3& vector)
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
    static std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
    static std::mt19937 generator;
    return min + distribution(generator) * (max - min);
  }

  inline bool Vec3EpsilonEqual(float epsilon, const Vec3& vec1, const Vec3& vec2)
  {
    return glm::all(glm::epsilonEqual(vec1, vec2, epsilon));
  }

  inline bool Vec3NearZero(const Vec3& vec)
  {
    static float min = 1e-8f;
    return (std::fabs(vec.x) < min && std::fabs(vec.y) < min && std::fabs(vec.z) < min);
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

  inline Vec3 RandomInUnitDisk()
  {
    while (true)
    {
      Vec3 p = Vec3(RandomFloat(-1.0f, 1.0f), RandomFloat(-1.0f, 1.0f), 0.0f);
      if (SquaredLength(p) >= 1.0f) continue;
      return p;
    }
  }

  /* 3D Math Operations */

  inline Vec3 Reflect(const Vec3& v1, const Vec3& v2)
  {
    return v1 - 2.0f * glm::dot(v1, v2) * v2;
  }

  inline Vec3 Refract(const Vec3& in, const Vec3& n, float etaInOverEtaOut)
  {
    float cosTheta = std::fmin(glm::dot(-in, n), 1.0f);
    Vec3 outRayPerpendicular = etaInOverEtaOut * (in + cosTheta * n);
    Vec3 outRayParallel =  -std::sqrtf(std::fabs(1.0f - SquaredLength(outRayPerpendicular))) * n;
    return outRayPerpendicular + outRayParallel;
  }

  inline float Reflectance(float cosine, float refractionRatio)
  {
    // Schlick's approximation for reflectance
    float r0 = (1.0f - refractionRatio) / (1.0f + refractionRatio);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * std::powf(1.0f - cosine, 5.0f);
  }

} // namespace WoohooRT
