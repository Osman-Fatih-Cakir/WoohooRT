
#include "MetalMaterial.hpp"

#include "../Engine/MathUtils.hpp"

namespace WoohooRT
{
  MetalMaterial::MetalMaterial(const Vec3& albedo, float fuzzy)
  {
    m_albedo = albedo;
    m_fuzzy = fuzzy;
  }

  bool MetalMaterial::Scatter(const Ray& rayIn, const Intersection& intersection, Vec3& attenuation, Ray& scattered) const
  {
    Vec3 reflected = Reflect(UnitVector(rayIn.m_direction), intersection.normal);
    Vec3 fuzzy = Vec3(0.0f);
    if (m_fuzzy != 0.0f)
    {
      fuzzy = m_fuzzy * RandomVec3InUnitSphere();
    }
    scattered = Ray(intersection.position, reflected + fuzzy);
    attenuation = m_albedo;
    return (glm::dot(scattered.m_direction, intersection.normal) > 0.0f);
  }

} // namespace WoohooRT
