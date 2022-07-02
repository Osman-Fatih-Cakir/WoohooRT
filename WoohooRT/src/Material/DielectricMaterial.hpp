#pragma once

#include "../Engine/Types.hpp"
#include "Material.hpp"

namespace WoohooRT
{
  class DielectricMaterial : public Material
  {
  public:
    DielectricMaterial(const Vec3& albedo, float ir);

    bool Scatter(const Ray& rayIn, const Intersection& intersection, Vec3& attenuation, Ray& scattered) const override;

  public:
    Vec3 m_albedo;
    float m_ir; // Index of refraction
  };

} // namespace WoohooRT
