#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "vec.hpp"

namespace rtm {
struct material {
  clr1 color{1, 1, 1};
  long double ambient{0.1};
  long double diffuse{0.9};
  long double specular{0.9};
  long double shininess{200.0};
};
} // namespace rtm

#endif
