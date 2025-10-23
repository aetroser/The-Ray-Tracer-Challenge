#ifndef HIT_HPP
#define HIT_HPP

#include "intersect.hpp"

namespace rtm {

constexpr std::optional<rtm::intersect> hit(const rtm::intersects &inters) {
  // Start with no hit found.
  std::optional<rtm::intersect> closest_hit;

  // Loop through every intersection in the provided list.
  for (const auto &current_intersection : inters) {
    // First, check if the intersection is in front of the ray (t >= 0).
    if (current_intersection.t >= 0) {
      // If this is the first valid hit we've found, or if this hit
      // is closer than the one we've saved, then update it.
      if (!closest_hit.has_value() || current_intersection.t < closest_hit->t) {
        closest_hit = current_intersection;
      }
    }
  }

  // Return the result. This will be the closest valid intersection,
  // or an empty optional if no valid intersections were found.
  return closest_hit;
}

} // namespace rtm

#endif
