
#include <cmath>

#include "DielectricMaterial.hpp"
#include "../Engine/MathUtils.hpp"

namespace WoohooRT
{
  DielectricMaterial::DielectricMaterial(const Vec3& albedo, float ir)
  {
    m_albedo = albedo;
    m_ir = ir;
  }

  bool DielectricMaterial::Scatter(const Ray& rayIn, const Intersection& intersection, Vec3& attenuation, Ray& scattered) const
  {
    // TODO This can be optimized to calculating this value before rendering starts (or maybe even calculating all permutations of refractionRatios)
    float refractionRatio = intersection.frontFace ? (1.0f / m_ir) : m_ir; // NOTE Assuming all rays come from air (which is not that great estimation I think)
    Vec3 unitDirection = UnitVector(rayIn.m_direction);

    float cosTheta = std::fmin(glm::dot(-unitDirection, UnitVector(intersection.normal)), 1.0f);
    float sinTheta = std::sqrtf(1.0f - cosTheta * cosTheta);

    Vec3 direction;

    if (refractionRatio * sinTheta > 1.0f || Reflectance(cosTheta, refractionRatio) > RandomFloat()) // Check if the ray can be refracted
    {
      direction = Reflect(unitDirection, intersection.normal);
    }
    else
    {
      direction = Refract(unitDirection, intersection.normal, refractionRatio);
    }

    attenuation = m_albedo;
    scattered = Ray(intersection.position, direction);
    return true;
  }

} // namespace WoohooRT
