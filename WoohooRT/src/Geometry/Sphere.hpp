#pragma once

#include "Geometry.hpp"
#include "../Engine/Types.hpp"

namespace WoohooRT
{
  class Sphere : public Geometry
  {
  public:
    Sphere(Vec3 m_position, float radius);

    bool Hit(const Ray& ray, float tMin, float tMax, Intersection& intersection) const override;

  private:
    Sphere() {};

  public:
    float m_radius;
  };

} // namespace WoohooRT
