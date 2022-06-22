#pragma once

#include "../Engine/Types.hpp"
#include "../Engine/Ray.hpp"
#include "Material.hpp"

namespace WoohooRT
{
  class LambertianMaterial : public Material
  {
  public:
    explicit LambertianMaterial(const Vec3& albedo);

    bool Scatter(const Ray& rayIn, const Intersection& intersection, Vec3& attenuation, Ray& scattered) const override;

  public:
    Vec3 m_albedo;
  };

} // namespace WoohooRT
