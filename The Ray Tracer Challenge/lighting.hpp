#ifndef LIGHTING_HPP
#define LIGHTING_HPP

#include "material.hpp"
#include "math_utils.hpp"
#include "sphere.hpp"
#include "vec.hpp"

namespace rtm {
struct point_light {
  clr1 intensity{1, 1, 1};
  vec4 position{0, 0, 0, 1};
};

constexpr clr1 lighting(const material &mat, const point_light &light,
                        const vec4 &point, const vec4 &eye_normal,
                        const normal &n) {
  auto effective_color = mat.color * light.intensity;
  auto lightv = normalize(light.position - point);

  auto ambient = effective_color * mat.ambient;
  clr1 diffuse = {0, 0, 0};
  clr1 specular = {0, 0, 0};

  auto light_dot_normal = dot_product(lightv, n);

  if (light_dot_normal < 0)
    return ambient + diffuse + specular;

  diffuse = effective_color * mat.diffuse * light_dot_normal;
  auto reflectv = reflect(-lightv, n);
  auto reflect_dot_eye = dot_product(reflectv, eye_normal);

  if (reflect_dot_eye > 0) {
    auto factor = power(reflect_dot_eye, mat.shininess);
    specular = light.intensity * mat.specular * factor;
  }

  return ambient + diffuse + specular;
}

} // namespace rtm

#endif
