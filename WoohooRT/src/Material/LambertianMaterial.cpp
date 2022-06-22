
#include "LambertianMaterial.hpp"

#include "../Engine/MathUtils.hpp"

namespace WoohooRT
{
  LambertianMaterial::LambertianMaterial(const Vec3& albedo)
  {
    m_albedo = albedo;
  }

  bool LambertianMaterial::Scatter(const Ray& rayIn, const Intersection& intersection, Vec3& attenuation, Ray& scattered) const
  {
    Vec3 scatterDirection = intersection.normal + RandomUnitVec3InUnitSphere();
    if (Vec3NearZero(scatterDirection)) // Zero check
    {
      scatterDirection = intersection.normal;
    }
    scattered = Ray(intersection.position, scatterDirection);
    attenuation = m_albedo;
    return true;
  }
} // namespace WoohooRT
