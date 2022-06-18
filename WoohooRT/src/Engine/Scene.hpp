#pragma once

#include <memory>
#include <vector>

#include "Types.hpp"
#include "Ray.hpp"
#include "../Geometry/Geometry.hpp"

namespace WoohooRT
{
  class Scene
  {
  public:
    Scene() { }

    inline void AddGeometry(std::shared_ptr<Geometry> geometry)
    {
      m_allGeometries.push_back(geometry);
    }

    inline void ClearScene()
    {
      m_allGeometries.clear();
    }

    inline std::vector<std::shared_ptr<Geometry>> GetGeometry()
    {
      return m_allGeometries;
    }

    inline bool Hit(const Ray& ray, float tMin, float tMax, Intersection& intersection)
    {
      Intersection tempIntersection; // TODO why? and optimize out
      bool hitAnything = false;
      float closestSoFar = tMax;

      for (const auto geometry : m_allGeometries)
      {
        if (geometry->Hit(ray, tMin, tMax, tempIntersection))
        {
          hitAnything = true;
          if (closestSoFar > tempIntersection.t)
          {
            closestSoFar = tempIntersection.t;
            intersection = tempIntersection;
          }
        }
      }

      return hitAnything;
    }

  private:
    std::vector<std::shared_ptr<Geometry>> m_allGeometries;

  };

} // namespace WoohooRT
