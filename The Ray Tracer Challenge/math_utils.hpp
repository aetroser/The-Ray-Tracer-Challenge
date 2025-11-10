#ifndef NUMERICS_HPP
#define NUMERICS_HPP
#include <fstream>
#include <numbers>

namespace rtm {
namespace constants {
inline constexpr size_t VEC_SPACING{9};
inline constexpr size_t MATRIX_SPACING{12};

inline constexpr size_t PRECISION{12};
inline constexpr long double EPSILON{
    1e-6}; // std::numeric_limits<long double>::epsilon()
inline constexpr long double PI{std::numbers::pi_v<long double>};
inline constexpr long double TWO_PI{2.l * PI};
inline constexpr long double HALF_PI{.5l * PI};
} // namespace constants

template <typename T>
  requires(std::is_trivial_v<T>)
constexpr T c_abs(T x) noexcept {
  return x < T{} ? -x : x;
}

template <typename T>
  requires(std::is_trivial_v<T>)
constexpr T c_max(T lhs, T rhs) noexcept {
  return (lhs < rhs) ? lhs : rhs;
}

template <std::floating_point T>
[[nodiscard]] constexpr bool are_close(const T lhs, const T rhs, const T epsilon = static_cast<T>(constants::EPSILON)) {
  if (lhs == rhs) // exact
    return true;

  const T diff = rtm::c_abs(lhs - rhs); // absolute
  if (diff <= epsilon)
    return true;

  return diff <=
         epsilon * rtm::c_max(rtm::c_abs(lhs), rtm::c_abs(rhs)); // relative
}

[[nodiscard]] constexpr long double radians(const long double deg) {
  return deg / 180 * constants::PI;
}

[[nodiscard]] constexpr long double degrees(const long double radians) {
  return radians * 180 / constants::PI;
}

template <std::floating_point T> constexpr T power(T base, int exp) noexcept {
  if (exp == 0)
    return 1;
  T result = 1;
  for (int i = 0; i < exp; ++i) {
    result *= base;
  }
  return result;
}

template <std::floating_point T> constexpr T factorial(int n) noexcept {
  if (n == 0)
    return 1;
  T result = 1;
  for (int i = 1; i <= n; ++i) {
    result *= i;
  }
  return result;
}

template <std::floating_point T> [[nodiscard]] constexpr T c_sin(T x) noexcept {
  // Normalize x to the range [-PI, PI]
  while (x > constants::PI)
    x -= constants::TWO_PI;
  while (x < -constants::PI)
    x += constants::TWO_PI;

  T result = 0;
  for (int n = 0; n < 10; ++n) {
    result += power(static_cast<T>(-1), n) * power(x, 2 * n + 1) /
              factorial<T>(2 * n + 1);
  }
  return result;
}

template <std::floating_point T> [[nodiscard]] constexpr T c_cos(T x) noexcept {
  // Normalize x to the range [-PI, PI]
  while (x > constants::PI)
    x -= constants::TWO_PI;
  while (x < -constants::PI)
    x += constants::TWO_PI;

  T result = 0;
  for (int n = 0; n < 10; ++n) {
    result +=
        power(static_cast<T>(-1), n) * power(x, 2 * n) / factorial<T>(2 * n);
  }
  return result;
}

template <std::floating_point T> [[nodiscard]] constexpr T c_tan(T x) noexcept {
  return c_sin(x) / c_cos(x);
}

template <std::floating_point T>
[[nodiscard]] constexpr T c_sqrt(T x) noexcept {
  if (x < 0)
    return std::numeric_limits<T>::quiet_NaN();
  if (x == 0)
    return 0;

  T guess = x / 2.0;
  T prev_guess = 0;

  for (int i = 0; i < 20; ++i) {
    if (guess == prev_guess)
      break;
    prev_guess = guess;
    guess = (guess + x / guess) / 2.0;
  }
  return guess;
}

template <std::floating_point T>
[[nodiscard]] constexpr T c_atan(T x) noexcept {
  if (x == 0)
    return 0;
  if (x > 1)
    return constants::HALF_PI - c_atan(1 / x);
  if (x < -1)
    return -constants::HALF_PI - c_atan(1 / x);

  T result = 0;
  for (int n = 0; n < 15; ++n) {
    result += power(static_cast<T>(-1), n) * power(x, 2 * n + 1) / (2 * n + 1);
  }
  return result;
}

template <std::floating_point T>
[[nodiscard]] constexpr T c_asin(T x) noexcept {
  if (x > 1 || x < -1)
    return std::numeric_limits<T>::quiet_NaN();
  return c_atan(x / c_sqrt(1 - x * x));
}

template <std::floating_point T>
[[nodiscard]] constexpr T c_acos(T x) noexcept {
  if (x > 1 || x < -1)
    return std::numeric_limits<T>::quiet_NaN();
  return constants::HALF_PI - c_asin(x);
}
} // namespace rtm

#endif
