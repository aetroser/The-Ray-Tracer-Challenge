#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#include "scene_object.hpp"
#include <array>
#include <memory>

namespace rtm {
class object;

struct intersect {
  long double t{};
  // vec<4, long double> point;
  std::weak_ptr<object> object{};
  // vec<4, long double> normal;
};

using intersects = std::array<intersect, 2>;
} // namespace rtm

#endif
