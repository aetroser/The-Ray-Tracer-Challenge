#ifndef CANVAS_HPP
#define CANVAS_HPP
#include "vec.hpp"
#include <algorithm>
#include <array>
#include <fstream>
#include <vector>

namespace rtm {
template <size_t W, size_t H> class canvas {
  using buffer_color_type = uint8_t;
  static constexpr std::string_view FILE_SIG{"P3"};
  static constexpr std::string_view FILE_MAX_ALLOWED_COLOR_VALUE{"255"};

  static constexpr size_t BUFFER_THRESHOLD{128ULL * 128ULL};
  static constexpr size_t BUFFER_SIZE{W * H};

  using buffer_data_type = vec<3, buffer_color_type>;

  using buffer_type =
      std::conditional_t<BUFFER_SIZE <= BUFFER_THRESHOLD,
                         std::array<buffer_data_type, BUFFER_SIZE>,
                         std::vector<buffer_data_type>>;

  static constexpr auto buffer_initialize() {
    if constexpr (std::is_same_v<buffer_type,
                                 std::array<buffer_data_type, BUFFER_SIZE>>)
      return buffer_type{};
    if constexpr (std::is_same_v<buffer_type, std::vector<buffer_data_type>>)
      return buffer_type(BUFFER_SIZE);
  }

public:
  canvas() = default;

  ~canvas() {
    std::ofstream file{"out.ppm"};
    file << FILE_SIG << '\n';
    file << W << ' ' << H << '\n';
    file << FILE_MAX_ALLOWED_COLOR_VALUE << '\n';

    for (size_t r = 0; r < H; ++r) {
      for (size_t c = 0; c < W; ++c) {
        const auto &pixel = (*this)(r, c);
        // Cast to int to print the numerical value
        file << static_cast<int>(pixel.r()) << ' '
             << static_cast<int>(pixel.g()) << ' '
             << static_cast<int>(pixel.b()) << ' ';
      }
      file << '\n';
    }
  }

  canvas(const canvas &) = delete;
  canvas(canvas &&) noexcept = delete;
  constexpr canvas &operator=(const canvas &) = delete;
  constexpr canvas &operator=(canvas &&) noexcept = delete;

  [[nodiscard]] constexpr const buffer_data_type &operator()(size_t row,
                                                             size_t col) const {
    return m_canvas_buffer[row * W + col];
  }

  [[nodiscard]] constexpr buffer_data_type &operator()(size_t row, size_t col) {
    return m_canvas_buffer[row * W + col];
  }

private:
  buffer_type m_canvas_buffer{buffer_initialize()};
};
} // namespace rtm

#endif
