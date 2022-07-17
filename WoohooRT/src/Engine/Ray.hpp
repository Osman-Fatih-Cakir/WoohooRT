#pragma once

#include <memory>

#include "Types.hpp"
#include "../Material/Material.hpp"
#include "../Engine/MathUtils.hpp"

namespace WoohooRT
{
  class Ray
  {
  public:
    Ray();
    Ray(Vec3 origin, Vec3 direction);

    inline Vec3 At(float t) const
    {
      return m_origin + t * m_direction;
    }

  public:
    Vec3 m_origin;
    Vec3 m_direction;
  };

  struct Intersection
  {
    Vec3 position;
    Vec3 normal;
    std::shared_ptr<class Material> material;
    float t;
    bool frontFace;

    inline void setFaceNormal(const Ray& ray, const Vec3& outwardNormal)
    {
      frontFace = glm::dot(ray.m_direction, outwardNormal) < 0.0f;
      normal = UnitVector(frontFace ? outwardNormal : -outwardNormal);
    }
  };

} // namespace WoohooRT
