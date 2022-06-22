#pragma once

#include "Material.hpp"

namespace WoohooRT
{
  class MetalMaterial : public Material
  {
  public:
    MetalMaterial(const Vec3& albedo, float fuzzy = 0.0f);

    bool Scatter(const Ray& rayIn, const Intersection& intersection, Vec3& attenuation, Ray& scattered) const;

  public:
    Vec3 m_albedo;
    float m_fuzzy;
  };

} // namespace WoohooRT
