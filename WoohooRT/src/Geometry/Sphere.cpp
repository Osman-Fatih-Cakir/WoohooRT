
#include "Sphere.hpp"
#include "../Engine/MathUtils.hpp"

namespace WoohooRT
{
  Sphere::Sphere(Vec3 position, float radius)
  {
    m_position = position;
    m_radius = radius;
  }

  bool Sphere::Hit(const Ray& ray, float tMin, float tMax, Intersection& intersection) const
  {
    static Vec3 centerToOrigin;
    centerToOrigin = ray.m_origin - m_position;
    float a = SquaredLength(ray.m_direction);
    float halfB = glm::dot(centerToOrigin, ray.m_direction);
    float c = SquaredLength(centerToOrigin) - m_radius * m_radius;
    float discriminant = halfB * halfB - a * c;

    if (discriminant < 0.0f)
    {
      return false;
    }

    // Check limitations of t value
    float sqrtD = glm::sqrt(discriminant);
    float root = (-halfB - sqrtD) / a;
    if (root < tMin || root > tMax)
    {
      root = (-halfB + sqrtD) / a;
      if (root < tMin || tMax < root)
      {
        return false;
      }
    }

    intersection.t = root;
    intersection.position = ray.At(root);
    Vec3 outwardNormal = (intersection.position - m_position) / m_radius; // TODO optimize out
    intersection.setFaceNormal(ray, outwardNormal);

    return true;
  }

} // namespace WoohooRT
