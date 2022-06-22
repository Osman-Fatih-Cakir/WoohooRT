
#include "Scene.hpp"

namespace WoohooRT
{
  bool Scene::Hit(const Ray& ray, float tMin, float tMax, Intersection& intersection) const
  {
    Intersection tempIntersection;
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

} // namespace WoohooRT
