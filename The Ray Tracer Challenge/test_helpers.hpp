#ifndef TESTS_HPP
#define TESTS_HPP
#include "math_utils.hpp"
#include "matrix.hpp"
#include "ray.hpp"
#include "vec.hpp"
#include <functional>
#include <iostream>
#include <stdexcept>

namespace rtm::testing {
template <typename T1, typename T2>
std::ostream &operator<<(std::ostream &os, const std::pair<T1, T2> &p) {
  os << "(" << p.first << ", " << p.second << ")";
  return os;
}

template <typename Actual, typename Expected>
static constexpr void check_and_print(const Expected &expected_val,
                                      const Actual &actual) {
  // At compile-time, choose which comparison to use.
  if constexpr (std::is_floating_point_v<std::decay_t<Actual>> ||
                std::is_floating_point_v<std::decay_t<Expected>>) {
    // Use are_close for floating-point types
    if (!are_close(static_cast<long double>(actual),
                   static_cast<long double>(expected_val))) {
      std::cerr << "\n--- TEST FAILED ---\n";
      std::cerr << "  Precision used: EPSILON = " << constants::EPSILON << '\n';
      std::cerr << "  Expected:\n"
                << std::setprecision(constants::PRECISION) << expected_val
                << "\n";
      std::cerr << "  Got:\n"
                << std::setprecision(constants::PRECISION) << actual << "\n";
      std::cerr << "-------------------\n";
      throw std::logic_error("Precondition failed");
    }
    std::cout << "\n--- TEST PASSED ---\n";
    std::cout << "  Precision used: EPSILON = " << constants::EPSILON << '\n';
    std::cout << "  Expected:\n"
              << std::setprecision(constants::PRECISION) << expected_val
              << "\n";
    std::cout << "  Got:\n"
              << std::setprecision(constants::PRECISION) << actual << "\n";
    std::cout << "-------------------\n";
  } else {
    // Use operator!= for all other types (int, vec, matrix, pair, etc.)
    if (actual != expected_val) {
      std::cerr << "\n--- TEST FAILED ---\n";
      std::cerr << "  Expected:\n" << expected_val << "\n";
      std::cerr << "  Got:\n" << actual << "\n";
      std::cerr << "-------------------\n";
      throw std::logic_error("Precondition failed");
    }
    std::cout << "\n--- TEST PASSED ---\n";
    std::cout << "  Expected:\n" << expected_val << "\n";
    std::cout << "  Got:\n" << actual << "\n";
    std::cout << "-------------------\n";
  }
}

template <typename Actual, typename Expected>
static constexpr void expected(const Actual& actual_val, const Expected& expected_val) {
    check_and_print(expected_val, actual_val);
}

template <typename Callable, typename Expected, typename... Args>
    requires std::invocable<Callable, Args...> // The C++20 constraint
static constexpr void expected(Callable&& func, const Expected& expected_val, Args&&... args) {
    auto actual_val = std::invoke(std::forward<Callable>(func), std::forward<Args>(args)...);
    check_and_print(expected_val, actual_val);
}

static void perform_misc_tests() {
  {
    auto a = make_matrix<4, 4>(
        {3, -9, 7, 3, 3, -8, 2, -9, -4, 4, 4, 1, -6, 5, -1, 1});

    auto b =
        make_matrix<4, 4>({8, 2, 2, 2, 3, -1, 7, 0, 7, 0, 5, 4, 6, -2, 0, 5});

    auto c = a * b;

    expected(a,
             matrix_cast<int>(matrix_cast<long double>(c) * matrix_inverse(b)));
  }

  {
    ray<long double> some_ray{{2, 3, 4, 1}, {1, 0, 0, 0}};

    expected(vec<4, long double>{2, 3, 4, 1}, position(some_ray, 0));
    expected(vec<4, long double>{3, 3, 4, 1}, position(some_ray, 1));
    expected(vec<4, long double>{1, 3, 4, 1}, position(some_ray, -1));
    expected(vec<4, long double>{4.5, 3, 4, 1}, position(some_ray, 2.5));

    some_ray.direction.y() = 1;

    expected(vec<4, long double>{2, 3, 4, 1}, position(some_ray, 0));
    expected(vec<4, long double>{3, 4, 4, 1}, position(some_ray, 1));
    expected(vec<4, long double>{1, 2, 4, 1}, position(some_ray, -1));
    expected(vec<4, long double>{4.5, 5.5, 4, 1}, position(some_ray, 2.5));

    some_ray.direction = {0, 0, 1, 0};

    expected(vec<4, long double>{2, 3, 4, 1}, position(some_ray, 0));
    expected(vec<4, long double>{2, 3, 5, 1}, position(some_ray, 1));
    expected(vec<4, long double>{2, 3, 3, 1}, position(some_ray, -1));
    expected(vec<4, long double>{2, 3, 6.5, 1}, position(some_ray, 2.5));
  }
}
} // namespace rtm::testing

#endif
