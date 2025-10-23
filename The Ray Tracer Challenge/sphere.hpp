#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "material.hpp"
#include "math_utils.hpp"
#include "scene_object.hpp"
#include "vec.hpp"
#include <memory>

namespace rtm {
class sphere : public object {
public:
  sphere() = default;

  static std::shared_ptr<sphere> make() { return std::make_shared<sphere>(); }

  material properties{};

protected:
  [[nodiscard]] constexpr std::optional<rtm::intersects>
  local_intersect(const rtm::ray<long double> &local_ray) override {
    // The vector from the sphere's center (0,0,0) to the ray's origin.
    auto sphere_to_ray = local_ray.origin - vec4{0, 0, 0, 1};

    // The standard ray-sphere intersection formula's components (a, b, c).
    long double a = dot_product(local_ray.direction, local_ray.direction);
    long double b = 2 * dot_product(local_ray.direction, sphere_to_ray);
    long double c = dot_product(sphere_to_ray, sphere_to_ray) -
                    1; // -1 because it's a unit sphere (radius^2 = 1).

    long double discriminant = b * b - 4 * a * c;

    // If the discriminant is negative, the ray misses the sphere.
    if (discriminant < 0) {
      return {}; // Return an empty vector of intersections.
    }

    // Calculate the two intersection points (t values).
    auto sqrt_discriminant = c_sqrt(discriminant);
    auto t1 = (-b - sqrt_discriminant) / (2 * a);
    auto t2 = (-b + sqrt_discriminant) / (2 * a);

    // Use shared_from_this() to safely get a shared_ptr to this object.
    // This is then implicitly converted to the weak_ptr in the intersect
    // struct.
    return {{{{t1, shared_from_this()}, {t2, shared_from_this()}}}};
  }
};

using normal = vec4;
using sphere_obj = std::shared_ptr<sphere>;

constexpr normal normal_at(const sphere_obj &sph, const vec4 &pt) {
  vec4 object_point = sph->inverse_transform() * pt;
  vec4 object_normal = object_point - vec4{0, 0, 0, 1};

  vec4 world_normal =
      matrix_transpose(sph->inverse_transform()) * object_normal;

  world_normal.w() = 0;

  return normalize(world_normal);
}

constexpr vec4 reflect(const vec4 &in, const normal &n) {
  return in - n * 2.L * dot_product(in, n);
}
} // namespace rtm

#endif
