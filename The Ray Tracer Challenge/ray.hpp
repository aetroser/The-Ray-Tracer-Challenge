#ifndef RAY_HPP
#define RAY_HPP

#include "matrix.hpp"
#include "vec.hpp"
#include <algorithm>

namespace rtm
{
	template <typename T = long double>
	struct ray
	{
		vec<4, T> origin{T{0}, T{0}, T{0}, T{1}};
		vec<4, T> direction{T{0}, T{0}, T{0}, T{0}};

		[[nodiscard]] constexpr bool operator==(const ray& rhs) const requires(std::floating_point<T>)
		{
			auto close = [](const vec<4, T>& v1, const vec<4, T>& v2)
			{
				return std::ranges::equal(v1, v2, [](const T& a, const T& b)
				{
					return rtm::are_close(a, b);
				});
			};

			return close(origin, rhs.origin) && close(direction, rhs.direction);
		}

		[[nodiscard]] constexpr bool operator==(const ray& rhs) const requires (!std::floating_point<T>) = default;

		constexpr void transform(const matrix<4, 4, T>& mat)
		{
			origin = mat * origin;
			direction = mat * direction;
		}
	};

	template <typename T>
	std::ostream& operator<<(std::ostream& os, const ray<T>& ray)
	{
		os << '{' << ray.origin << "}\t{" << ray.direction << "}";
		return os;
	}

	template <typename T>
	[[nodiscard]] constexpr vec<4, T> position(const ray<T>& r,
	                                           const long double t)
	{
		return vec<4, T>{r.origin + r.direction * t};
	}
} // namespace rtm

#endif
