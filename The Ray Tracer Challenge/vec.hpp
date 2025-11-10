#ifndef VEC_HPP
#define VEC_HPP
#include "math_utils.hpp"
#include <algorithm>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <ranges>
#include <stdexcept>

namespace rtm {
template <size_t N, typename T>
  requires(std::is_trivial_v<T> && N <= 4 && N > 0)
class vec;
}

template <typename U, size_t N, typename T>
[[nodiscard]] constexpr rtm::vec<N, U> vec_cast(const rtm::vec<N, T> &v) {
  rtm::vec<N, U> temporary;

  for (size_t i = 0; i < N; ++i)
    temporary[i] = static_cast<U>(v[i]);

  return temporary;
}

namespace rtm {
template <size_t N, typename T>
  requires(std::is_trivial_v<T> && N <= 4 && N > 0)
class vec {
public:
  constexpr vec() = default;

  constexpr ~vec() = default;

  // if we did explicit
  //	use wouldn't be able to do:
  //	 rtm::vec<4, int> a = {1, 2, 3, 4}
  constexpr vec(std::initializer_list<T> i_list) {
    if (std::size(i_list) != N)
      throw std::length_error(
          "Initializer list size does not match vector size");

    std::ranges::copy(i_list, m_data.begin());
  }

  constexpr vec(const vec &) = default;

  constexpr vec(vec &&) noexcept = default;

  template <typename U>
  constexpr vec(const vec<N, U> &v)
      : vec(vec_cast<T>(v)) // delegate to appropriate constructor
  {}

  [[nodiscard]] constexpr auto begin() noexcept { return m_data.begin(); }
  [[nodiscard]] constexpr auto begin() const noexcept { return m_data.begin(); }
  [[nodiscard]] constexpr auto cbegin() const noexcept { return m_data.cbegin(); }

  [[nodiscard]] constexpr auto end() noexcept { return m_data.end(); }
  [[nodiscard]] constexpr auto end() const noexcept { return m_data.end(); }
  [[nodiscard]] constexpr auto cend() const noexcept { return m_data.end(); }

  constexpr vec &operator=(const vec &) = default;

  constexpr vec &operator=(vec &&) noexcept = default;

  template <size_t I>
  constexpr const T &get() const
    requires(I < N)
  {
    return m_data[I];
  }

  template <size_t I>
  constexpr T &get()
    requires(I < N)
  {
    return m_data[I];
  }

  constexpr const T &operator[](size_t i) const noexcept { return m_data[i]; }

  constexpr T &operator[](size_t i) noexcept { return m_data[i]; }

  constexpr const T &x() const { return m_data[0]; }

  constexpr T &x() { return m_data[0]; }

  constexpr const T &r() const { return m_data[0]; }

  constexpr T &r() { return m_data[0]; }

  constexpr const T &y() const
    requires(N > 1)
  {
    return m_data[1];
  }

  constexpr T &y()
    requires(N > 1)
  {
    return m_data[1];
  }

  constexpr const T &g() const
    requires(N > 1)
  {
    return m_data[1];
  }

  constexpr T &g()
    requires(N > 1)
  {
    return m_data[1];
  }

  constexpr const T &z() const
    requires(N > 2)
  {
    return m_data[2];
  }

  constexpr T &z()
    requires(N > 2)
  {
    return m_data[2];
  }

  constexpr const T &b() const
    requires(N > 2)
  {
    return m_data[2];
  }

  constexpr T &b()
    requires(N > 2)
  {
    return m_data[2];
  }

  constexpr const T &w() const
    requires(N > 3)
  {
    return m_data[3];
  }

  constexpr T &w()
    requires(N > 3)
  {
    return m_data[3];
  }

  // comparison operators

  [[nodiscard]] constexpr bool operator==(const vec &rhs) const
    requires(std::floating_point<T>)
  {
    return std::ranges::equal(m_data, rhs.m_data, [](const T &a, const T &b) {
      return are_close(a, b);
    });
  }

  [[nodiscard]] constexpr bool operator==(const vec &) const
    requires(!std::floating_point<T>)
  = default;

  // modifying (*this) operators

  constexpr vec &operator+=(const vec &rhs) {
    for (size_t i = 0; i < N; ++i)
      m_data[i] += rhs.m_data[i];

    return *this;
  }

  constexpr vec &operator+=(const T scalar) {
    for (size_t i = 0; i < N; ++i)
      m_data[i] += scalar;

    return *this;
  }

  constexpr vec &operator-=(const vec &rhs) {
    for (size_t i = 0; i < N; ++i)
      m_data[i] -= rhs.m_data[i];

    return *this;
  }

  constexpr vec &operator-=(const T scalar) {
    for (size_t i = 0; i < N; ++i)
      m_data[i] -= scalar;

    return *this;
  }

  constexpr vec operator-() const {
    vec temporary;
    for (size_t i = 0; i < N; ++i)
      temporary.m_data[i] = -m_data[i];

    return temporary;
  }

  constexpr vec &operator*=(const vec &rhs) {
    for (size_t i = 0; i < N; ++i)
      m_data[i] *= rhs.m_data[i];

    return *this;
  }

  constexpr vec &operator*=(const T scalar) {
    for (size_t i = 0; i < N; ++i)
      m_data[i] *= scalar;

    return *this;
  }

  constexpr vec &operator/=(const vec &rhs) {
    for (size_t i = 0; i < N; ++i)
      m_data[i] /= rhs.m_data[i];

    return *this;
  }

  constexpr vec &operator/=(const T scalar) {
    for (size_t i = 0; i < N; ++i)
      m_data[i] /= scalar;

    return *this;
  }

  constexpr vec &operator%=(const vec &rhs)
    requires(std::integral<T>)
  {
    for (size_t i = 0; i < N; ++i)
      m_data[i] %= rhs.m_data[i];

    return *this;
  }

  constexpr vec &operator%=(const T scalar)
    requires(std::integral<T>)
  {
    for (size_t i = 0; i < N; ++i)
      m_data[i] %= scalar;

    return *this;
  }

  friend std::ostream &operator<<(std::ostream &os, const vec &rhs) {
    os << rhs.m_data[0];
    for (size_t i = 1; i < N; ++i)
      os << std::setw(constants::VEC_SPACING)
         << std::setprecision(constants::PRECISION) << rhs.m_data[i];

    return os;
  }

private:
  std::array<T, N> m_data{};
};

template <size_t N, typename T>
constexpr vec<N, T> operator+(vec<N, T> lhs, const vec<N, T> &rhs) {
  return lhs += rhs;
}

template <size_t N, typename T>
constexpr vec<N, T> operator+(vec<N, T> lhs, const T scalar) {
  return lhs += scalar;
}

template <size_t N, typename T>
constexpr vec<N, T> operator-(vec<N, T> lhs, const vec<N, T> &rhs) {
  return lhs -= rhs;
}

template <size_t N, typename T>
constexpr vec<N, T> operator-(vec<N, T> lhs, const T scalar) {
  return lhs -= scalar;
}

template <size_t N, typename T>
constexpr vec<N, T> operator*(vec<N, T> lhs, const vec<N, T> &rhs) {
  return lhs *= rhs;
}

template <size_t N, typename T>
constexpr vec<N, T> operator*(vec<N, T> lhs, const T scalar) {
  return lhs *= scalar;
}

template <size_t N, typename T>
constexpr vec<N, T> operator/(vec<N, T> lhs, const vec<N, T> &rhs) {
  return lhs /= rhs;
}

template <size_t N, typename T>
constexpr vec<N, T> operator/(vec<N, T> lhs, const T scalar) {
  return lhs /= scalar;
}

template <size_t N, typename T>
  requires(std::integral<T>)
constexpr vec<N, T> operator%(vec<N, T> lhs, const vec<N, T> &rhs) {
  return lhs %= rhs;
}

template <size_t N, typename T>
  requires(std::integral<T>)
constexpr vec<N, T> operator%(vec<N, T> lhs, const T scalar) {
  return lhs %= scalar;
}

template <typename T = void> struct Magnitude {
  template <size_t N, typename U>
  [[nodiscard]] constexpr auto operator()(const vec<N, U> &v) const {
    long double sum = 0.;
    for (size_t i = 0; i < N; ++i) {
      sum += static_cast<long double>(v[i] * v[i]);
    }
    return std::sqrt(sum);
  }
};

template <size_t N, typename T>
constexpr long double magnitude(const vec<N, T> &v) {
  return Magnitude{}(v);
}

template <typename T = void> struct Normalize {
  template <size_t N, typename U>
  [[nodiscard]] constexpr auto operator()(const vec<N, U> &v) {
    vec<N, long double> temporary(v);
    const auto current_magnitude = magnitude(v);

    for (size_t i = 0; i < N; ++i)
      temporary[i] /= current_magnitude;

    return temporary;
  }
};

template <typename U = long double, size_t N, typename T>
constexpr vec<N, U> normalize(vec<N, T> v) {
  return Normalize{}(v);
}

template <typename T = void> struct DotProduct {
  template <size_t N, typename U>
  [[nodiscard]] constexpr U operator()(const vec<N, U> &a,
                                       const vec<N, U> &b) const {
    U temporary{};

    for (size_t i = 0; i < N; ++i)
      temporary += a[i] * b[i];

    return temporary;
  }
};

template <size_t N, typename T>
constexpr T dot_product(const vec<N, T> &a, const vec<N, T> &b) {
  return DotProduct{}(a, b);
}

template <typename T = void> struct CrossProduct {
  template <typename U>
  [[nodiscard]] constexpr vec<4, U> operator()(const vec<4, U> &a,
                                               const vec<4, U> &b) const {
    return rtm::vec<4, U>{a.y() * b.z() - a.z() * b.y(),
                          a.z() * b.x() - a.x() * b.z(),
                          a.x() * b.y() - a.y() * b.x(), U{}};
  }
};

template <typename T>
constexpr vec<4, T> cross_product(const vec<4, T> &a, const vec<4, T> b) {
  return CrossProduct{}(a, b);
}

using vec3 = vec<3, long double>;
using vec4 = vec<4, long double>;
using clr255 = vec<3, uint8_t>;
using clr1 = vec<3, long double>;

namespace constants {
inline constexpr clr255 RED255{255, 0, 0};
inline constexpr clr255 GRN255{0, 255, 0};
inline constexpr clr255 BLU255{0, 0, 255};

inline constexpr clr1 RED1{1.L, 0.L, 0.L};
inline constexpr clr1 GRN1{0.L, 1.L, 0.L};
inline constexpr clr1 BLU1{0.L, 0.L, 1.L};
} // namespace constants
} // namespace rtm
#endif
