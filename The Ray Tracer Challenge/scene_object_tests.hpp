#ifndef SCENE_OBJECT_TESTS_HPP
#define SCENE_OBJECT_TESTS_HPP

#include "hit.hpp"
#include "lighting.hpp"
#include "sphere.hpp"
#include "test_helpers.hpp"

namespace rtm::testing {
inline void perform_scene_tests() {
  // Ray intersect with sphere test
  {
    rtm::ray<long double> some_ray{{0, 0, -5, 1}, {0, 0, 1, 0}};

    auto some_sphere = rtm::sphere::make();

    rtm::intersects some_intersect = some_sphere->intersect(some_ray).value();

    std::pair ret_vals{some_intersect[0].t, some_intersect[1].t};

    testing::expected(std::pair{4.L, 6.L}, ret_vals);

    some_ray.origin = {0, 1, -5, 1};

    some_intersect = some_sphere->intersect(some_ray).value();

    auto ret_val = some_intersect[0].t;

    testing::expected(5.L, ret_val);

    some_ray.origin = {0, 0, 0, 1};

    some_intersect = some_sphere->intersect(some_ray).value();

    ret_vals = {some_intersect[0].t, some_intersect[1].t};

    testing::expected(std::pair{-1.L, 1.L}, ret_vals);

    some_ray.origin = {0, 0, 5, 1};

    some_intersect = some_sphere->intersect(some_ray).value();

    ret_vals = {some_intersect[0].t, some_intersect[1].t};

    testing::expected(std::pair{-6.L, -4.L}, ret_vals);
  }

  {
    auto some_sphere = rtm::sphere::make();

    rtm::intersect i1 = {5, some_sphere};
    rtm::intersect i2 = {7, some_sphere};
    rtm::intersect i3 = {-3, some_sphere};
    rtm::intersect i4 = {2, some_sphere};

    rtm::intersect out;

    for (const auto &some_intersect :
         {rtm::intersects{i1, i2}, rtm::intersects{i3, i4}}) {
      auto a = hit(some_intersect);

      if (a.has_value())
        out = a.value();
    }

    testing::expected(i4.t, out.t);
  }

  {
    auto some_ray =
        rtm::ray<long double>{{1.L, 2.L, 3.L, 1.L}, {0.L, 1.L, 0.L, 0.L}};

    auto some_mat = rtm::matrix_translate({3, 4, 5});

    some_ray.transform(some_mat);

    testing::expected(
        rtm::ray<long double>{{4.L, 6.L, 8.L, 1.L}, {0.L, 1.L, 0.L, 0.L}},
        some_ray);
  }

  {
    auto some_sphere = rtm::sphere::make();

    testing::expected(identity_matrix<4, long double>(),
                      some_sphere->transform());

    auto some_translation = matrix_translate({2, 3, 4});

    some_sphere->set_transform(some_translation);

    testing::expected(some_translation, some_sphere->transform());
  }

  {
    auto some_sphere = rtm::sphere::make();
    auto some_ray =
        rtm::ray<long double>{{0.L, 0.L, -5.L, 1.L}, {0.L, 0.L, 1.L, 0.L}};

    some_sphere->set_transform(matrix_scale({2, 2, 2}));

    auto some_intersects = some_sphere->intersect(some_ray).value();

    std::pair some_results = {some_intersects[0].t, some_intersects[1].t};

    testing::expected(std::pair{3.L, 7.L}, some_results);
  }

  {
    auto some_sphere = rtm::sphere::make();
    auto some_ray =
        rtm::ray<long double>{{0.L, 0.L, -5.L, 1.L}, {0.L, 0.L, 1.L, 0.L}};

    some_sphere->set_transform(matrix_translate({5, 0, 0}));

    auto some_intersects = some_sphere->intersect(some_ray);

    testing::expected(false, some_intersects.has_value());
  }

  {
    auto some_sphere = rtm::sphere::make();

    normal n = rtm::normal_at(some_sphere, vec4{1, 0, 0, 1});

    testing::expected(normal{1, 0, 0, 0}, n);

    n = rtm::normal_at(some_sphere, vec4{0, 1, 0, 1});

    testing::expected(normal{0, 1, 0, 0}, n);

    n = rtm::normal_at(some_sphere, vec4{0, 0, 1, 1});

    testing::expected(normal{0, 0, 1, 0}, n);

    n = rtm::normal_at(some_sphere, normal{c_sqrt(3.L) / 3, c_sqrt(3.L) / 3,
                                           c_sqrt(3.L) / 3, 1});

    testing::expected(
        normal{c_sqrt(3.L) / 3, c_sqrt(3.L) / 3, c_sqrt(3.L) / 3, 0}, n);

    testing::expected(n, normalize(n));

    some_sphere->set_transform(matrix_translate({0, 1, 0}));

    n = rtm::normal_at(some_sphere, vec4{0, 1.70711, -0.70711, 1});

    testing::expected(normal{0, 0.707106781187, -0.707106781187, 0}, n);
  }

  {
    auto some_sphere = rtm::sphere::make();

    vec4 n;

    some_sphere->set_transform(matrix_scale({1, 0.5, 1}) *
                               matrix_rotate_z(constants::PI / 5));

    n = rtm::normal_at(some_sphere,
                       vec4{0, c_sqrt(2.L) / 2, -c_sqrt(2.L) / 2, 1});

    testing::expected(vec4{0, 0.970142500145, -0.242535625036, 0}, n);

    testing::expected(vec4{1, 1, 0, 0},
                      rtm::reflect(vec4{1, -1, 0, 0}, normal{0, 1, 0, 0}));

    testing::expected(
        vec4{1, 0, 0, 0},
        rtm::reflect(vec4{0, -1, 0, 0},
                     normal{c_sqrt(2.L) / 2, c_sqrt(2.L) / 2, 0, 0}));
  }

  {
    material m{};
    vec4 eyev{0, 0, -1, 0};
    vec4 normalv{0, 0, -1, 0};
    point_light light{{1, 1, 1}, {0, 0, -10, 1}};
    testing::expected(clr1{1.9, 1.9, 1.9},
                      lighting(m, light, vec4{0, 0, 0, 1}, eyev, normalv));

    eyev = {0, c_sqrt(2.L) / 2, c_sqrt(2.L) / 2, 0};
    normalv = {0, 0, -1, 0};
    testing::expected(clr1{1, 1, 1},
                      lighting(m, light, vec4{0, 0, 0, 1}, eyev, normalv));

    eyev = {0, 0, -1, 0};
    normalv = {0, 0, -1, 0};
    light.position.y() = 10;
    testing::expected(clr1{0.736396103068, 0.736396103068, 0.736396103068},
                      lighting(m, light, vec4{0, 0, 0, 1}, eyev, normalv));

    eyev = {0, -c_sqrt(2.L) / 2, -c_sqrt(2.L) / 2, 0};
    testing::expected(clr1{1.63639610307, 1.63639610307, 1.63639610307},
                      lighting(m, light, vec4{0, 0, 0, 1}, eyev, normalv));

    eyev = {0, 0, -1, 0};
    light.position = {0, 0, 10, 1};
    testing::expected(clr1{0.1, 0.1, 0.1},
                      lighting(m, light, vec4{0, 0, 0, 1}, eyev, normalv));
  }
}
} // namespace rtm::testing

#endif
