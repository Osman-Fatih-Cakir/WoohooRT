
#include "Sphere.hpp"

#include "../Engine/MathUtils.hpp"

namespace WoohooRT
{
  Sphere::Sphere(Vec3 position, float radius, std::shared_ptr<Material> material)
  {
    m_position = position;
    m_radius = radius;
    m_material = material;
  }

  bool Sphere::Hit(const Ray& ray, float tMin, float tMax, Intersection& intersection) const
  {
    static Vec3 centerToOrigin;
    centerToOrigin = ray.m_origin - m_position;
    // I implemented the dot and SquareLength vectors because those were taking less time according to Visual Studio profiler
    // and this function is on the hottest path for this program
    //float a = SquaredLength(ray.m_direction);
    //float halfB = glm::dot(centerToOrigin, ray.m_direction);
    float a = ray.m_direction.x * ray.m_direction.x + ray.m_direction.y * ray.m_direction.y + ray.m_direction.z * ray.m_direction.z;
    float halfB = centerToOrigin.x * ray.m_direction.x + centerToOrigin.y * ray.m_direction.y + centerToOrigin.z * ray.m_direction.z;
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
    Vec3 outwardNormal = (intersection.position - m_position) / m_radius;
    intersection.setFaceNormal(ray, outwardNormal);
    intersection.material = m_material;

    return true;
  }

} // namespace WoohooRT
