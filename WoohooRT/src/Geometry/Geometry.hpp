#pragma once

#include <memory>

#include "../Engine/Ray.hpp"

namespace WoohooRT
{
  class Geometry
  {
  public:
    virtual bool Hit(const Ray& ray, float tMin, float tMax, Intersection& intersection) const = 0;

  protected:
    Geometry() { };

  public:
    Vec3 m_position;
    std::shared_ptr<Material> m_material;
  };

} // namespace WoohooRT
