#pragma once

#include <memory>

#include "Geometry.hpp"
#include "../Engine/Types.hpp"

namespace WoohooRT
{
  class Sphere final : public Geometry
  {
  public:
    Sphere(Vec3 m_position, float radius, std::shared_ptr<Material> material);

    bool Hit(const Ray& ray, float tMin, float tMax, Intersection& intersection) const override;

  private:
    Sphere() {};

  public:
    float m_radius;
  };

} // namespace WoohooRT
