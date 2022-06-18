#pragma once

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
  };

} // namespace WoohooRT
