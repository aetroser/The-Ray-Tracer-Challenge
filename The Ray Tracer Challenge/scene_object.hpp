#ifndef SCENE_OBJECT_HPP
#define SCENE_OBJECT_HPP

#include "intersect.hpp"
#include "matrix.hpp"
#include "ray.hpp"

namespace rtm {
class object : public std::enable_shared_from_this<object> {
public:
  constexpr object() = default;

  virtual ~object() = default;

  constexpr object(const object &) = delete;

  constexpr object &operator=(const object &) = delete;

  constexpr object(object &&) noexcept = delete;

  constexpr object &operator=(object &&) noexcept = delete;

  [[nodiscard]] constexpr const rtm::matrix<4, 4, long double> &
  transform() const {
    return m_transform;
  }

  [[nodiscard]] constexpr const rtm::matrix<4, 4, long double> &
  inverse_transform() const {
    return m_inverse_transform;
  }

  [[nodiscard]] std::optional<rtm::intersects>
  intersect(const rtm::ray<long double> &ray) {
    auto local_ray = transform_ray(ray, m_inverse_transform);

    return local_intersect(local_ray);
  }

  constexpr void
  set_transform(const rtm::matrix<4, 4, long double> &transform) {
    m_transform = transform;
    m_inverse_transform = rtm::matrix_inverse(transform);
  }

protected:
  [[nodiscard]] virtual constexpr std::optional<rtm::intersects>
  local_intersect(const rtm::ray<long double> &local_ray) = 0;

private:
  rtm::matrix<4, 4, long double> m_transform{identity_matrix<4, long double>()};
  rtm::matrix<4, 4, long double> m_inverse_transform{
      identity_matrix<4, long double>()};

  static rtm::ray<long double>
  transform_ray(const rtm::ray<long double> &ray,
                const rtm::matrix<4, 4, long double> &mat) {
    return {.origin = mat * ray.origin, .direction = mat * ray.direction};
  }
};
} // namespace rtm

#endif
