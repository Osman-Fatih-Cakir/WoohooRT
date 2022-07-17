#pragma once

#include "../Engine/Types.hpp"
#include "../Engine/Ray.hpp"

namespace WoohooRT
{
  class Ray;
  struct Intersection;

  class Material
  {
  public:
    virtual bool Scatter(const Ray& rayIn, const Intersection& intersection, Vec3& attenuation, Ray& scattered) const = 0;

  protected:
    Material() {}
  };

} // namespace WoohooRT
