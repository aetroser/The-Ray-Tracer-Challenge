#ifndef MATRIX_HPP
#define MATRIX_HPP

#include <algorithm>
#include <array>
#include <concepts>
#include <iomanip>
#include <ranges>
#include <stdexcept>
#include <type_traits>

#include "math_utils.hpp"
#include "vec.hpp"

namespace rtm
{
	template <size_t R, size_t C, typename T>
		requires std::is_trivial_v<T> && (R * C <= 16)
	class matrix;

	template <typename T1, typename T2, size_t R, size_t C>
		requires std::is_convertible_v<T2, T1>
	[[nodiscard]] constexpr matrix<R, C, T1>
	matrix_cast(const matrix<R, C, T2>& mat);

	template <size_t R, size_t C, typename T>
		requires std::is_trivial_v<T> && (R * C <= 16)
	class matrix
	{
	public:
		constexpr matrix() = default;

		constexpr ~matrix() = default;

		constexpr matrix(std::initializer_list<T> i_list)
		{
			if (std::size(i_list) != R * C)
				throw std::length_error(
					"Initializer list size does not match matrix size");

			std::ranges::copy(i_list, std::begin(m_linear_buffer));
		}

		constexpr matrix(const matrix&) = default;

		constexpr matrix(matrix&&) noexcept = default;

		template <typename T2>
		constexpr matrix(const matrix<R, C, T2>& mat) : matrix(matrix_cast<T>(mat))
		{
		}

		constexpr matrix& operator=(const matrix&) = default;

		constexpr matrix& operator=(matrix&&) noexcept = default;

		[[nodiscard]] constexpr const T& operator()(const size_t row,
		                                            const size_t col) const
		{
			return m_linear_buffer[row * C + col];
		}

		[[nodiscard]] constexpr T& operator()(const size_t row, const size_t col)
		{
			return m_linear_buffer[row * C + col];
		}

		[[nodiscard]] constexpr bool operator==(const matrix& rhs) const
		{
			if constexpr (std::floating_point<T>)
			{
				return std::ranges::equal(
					m_linear_buffer, rhs.m_linear_buffer, [](const T& a, const T& b)
					{
						return are_close(a, b); // currently EPSILON = 1e-6
					});
			}
			else
			{
				return std::ranges::equal(m_linear_buffer, rhs.m_linear_buffer);
			}
		}

		[[nodiscard]] constexpr bool operator!=(const matrix& rhs) const
		{
			if constexpr (std::floating_point<T>)
			{
				return !std::ranges::equal(
					m_linear_buffer, rhs.m_linear_buffer, [](const T& a, const T& b)
					{
						return are_close(a, b); // currently EPSILON = 1e-6
					});
			}
			else
			{
				return !std::ranges::equal(m_linear_buffer, rhs.m_linear_buffer);
			}
		}

		[[nodiscard]] constexpr bool operator<(const matrix&) const = delete;

		[[nodiscard]] constexpr bool operator<=(const matrix&) const = delete;

		[[nodiscard]] constexpr bool operator>(const matrix&) const = delete;

		[[nodiscard]] constexpr bool operator>=(const matrix&) const = delete;

		constexpr matrix& operator+=(const matrix& rhs)
		{
			for (size_t i = 0; i < R * C; ++i)
				m_linear_buffer[i] += rhs.m_linear_buffer[i];

			return *this;
		}

		constexpr matrix& operator+=(const T scalar)
		{
			for (size_t i = 0; i < R * C; ++i)
				m_linear_buffer[i] += scalar;

			return *this;
		}

		constexpr matrix& operator-=(const matrix& rhs)
		{
			for (size_t i = 0; i < R * C; ++i)
				m_linear_buffer[i] -= rhs.m_linear_buffer[i];

			return *this;
		}

		constexpr matrix& operator-=(const T scalar)
		{
			for (size_t i = 0; i < R * C; ++i)
				m_linear_buffer[i] -= scalar;

			return *this;
		}

		constexpr matrix& operator/=(const matrix&) = delete;

		constexpr matrix& operator/=(const T scalar)
		{
			if constexpr (std::floating_point<T>)
			{
				if (are_close(scalar, T{}))
					throw std::domain_error("Division by near-zero");
			}
			else
			{
				if (scalar == T{})
					throw std::domain_error("Division by zero");
			}

			for (size_t i = 0; i < R * C; ++i)
				m_linear_buffer[i] /= scalar;

			return *this;
		}

		constexpr matrix& operator%=(const matrix&) = delete;

		constexpr matrix operator/(const matrix&) = delete;

		constexpr matrix operator%(const matrix&) = delete;

		constexpr matrix operator-() const
		{
			matrix temporary{};

			for (size_t i = 0; i < R * C; ++i)
				temporary.m_linear_buffer[i] = -m_linear_buffer[i];

			return temporary;
		}

		constexpr auto begin() const noexcept { return m_linear_buffer.begin(); }

		constexpr auto begin() noexcept { return m_linear_buffer.begin(); }

		constexpr auto end() const noexcept { return m_linear_buffer.end(); }

		constexpr auto end() noexcept { return m_linear_buffer.end(); }

	private:
		std::array<T, R * C> m_linear_buffer{};
	};

	template <size_t E, typename T>
	constexpr matrix<E, E, T>& operator*=(matrix<E, E, T>& lhs,
	                                      const matrix<E, E, T>& rhs)
	{
		std::array<T, E> row_buffer{};

		for (size_t r = 0; r < E; ++r)
		{
			T* row_begin = &lhs(r, 0);
			std::ranges::copy(row_begin, row_begin + E, std::begin(row_buffer));

			for (size_t c = 0; c < E; ++c)
			{
				T sum{};
				for (size_t k = 0; k < E; ++k)
					sum += row_buffer[k] * rhs(k, c);
				lhs(r, c) = sum;
			}
		}

		return lhs;
	}

	template <size_t R, size_t C, typename T>
	constexpr matrix<R, C, T>& operator*=(matrix<R, C, T>& lhs, const T scalar)
	{
		for (size_t r = 0; r < R; ++r)
			for (size_t c = 0; c < C; ++c)
				lhs(r, c) *= scalar;
		return lhs;
	}

	template <size_t R, size_t C, typename T>
	[[nodiscard]] constexpr matrix<R, C, T> operator+(matrix<R, C, T> lhs,
	                                                  const matrix<R, C, T>& rhs)
	{
		lhs += rhs;
		return lhs;
	}

	template <size_t R, size_t C, typename T>
	[[nodiscard]] constexpr matrix<R, C, T> operator+(matrix<R, C, T> lhs,
	                                                  const T scalar)
	{
		lhs += scalar;
		return lhs;
	}

	template <size_t R, size_t C, typename T>
	[[nodiscard]] constexpr matrix<R, C, T> operator-(matrix<R, C, T> lhs,
	                                                  const matrix<R, C, T>& rhs)
	{
		lhs -= rhs;
		return lhs;
	}

	template <size_t R, size_t C, typename T>
	[[nodiscard]] constexpr matrix<R, C, T> operator-(matrix<R, C, T> lhs,
	                                                  const T scalar)
	{
		lhs -= scalar;
		return lhs;
	}

	template <size_t E, typename T>
	[[nodiscard]] constexpr matrix<E, E, T> operator*(matrix<E, E, T> lhs,
	                                                  const matrix<E, E, T>& rhs)
	{
		lhs *= rhs;
		return lhs;
	}

	template <size_t R, size_t C1, size_t C2, typename T>
	[[nodiscard]] constexpr matrix<R, C2, T>
	operator*(const matrix<R, C1, T>& lhs, const matrix<C1, C2, T>& rhs)
	{
		matrix<R, C2, T> temporary{};

		for (size_t r = 0; r < R; ++r)
			for (size_t c = 0; c < C2; ++c)
				for (size_t k = 0; k < C1; ++k)
					temporary(r, c) += lhs(r, k) * rhs(k, c);

		return temporary;
	}

	template <size_t R, size_t C, typename T>
	[[nodiscard]] constexpr vec<R, T> operator*(const matrix<R, C, T>& lhs,
	                                            const vec<C, T>& rhs)
	{
		vec<R, T> temporary{};

		for (size_t r = 0; r < R; ++r)
			for (size_t c = 0; c < C; ++c)
				temporary[r] += lhs(r, c) * rhs[c];

		return temporary;
	}

	template <size_t R, size_t C, typename T>
	[[nodiscard]] constexpr matrix<R, C, T> operator*(matrix<R, C, T> lhs,
	                                                  const T scalar)
	{
		lhs *= scalar;
		return lhs;
	}

	template <size_t R, size_t C, typename T>
	[[nodiscard]] constexpr matrix<R, C, T> operator/(matrix<R, C, T> lhs,
	                                                  const T scalar)
	{
		lhs /= scalar;
		return lhs;
	}

	template <size_t R, size_t C, typename T>
	std::ostream& operator<<(std::ostream& os, const matrix<R, C, T>& rhs)
	{
		for (size_t r = 0; r < R; ++r)
		{
			for (size_t c = 0; c < C; ++c)
				os << std::setw(constants::MATRIX_SPACING) << rhs(r, c);
			os << '\n';
		}

		return os;
	}

	template <size_t E, typename T>
	struct IdentityMatrix
	{
		[[nodiscard]] consteval matrix<E, E, T> operator()()
		{
			matrix<E, E, T> temporary{};

			for (size_t i = 0; i < E; ++i)
				temporary(i, i) = static_cast<T>(1);
			return temporary;
		}
	};

	template <size_t E, typename T>
	[[nodiscard]] consteval matrix<E, E, T> identity_matrix()
	{
		return IdentityMatrix<E, T>{}();
	}

	template <size_t R, size_t C, typename T>
	[[nodiscard]] constexpr matrix<R, C, T>
	make_matrix(std::initializer_list<T> i_list)
	{
		// No need for size check of initializer list as we already have that inside
		// of the appropriate constructor of matrix<...>
		return rtm::matrix<R, C, T>{i_list};
	}

	template <typename T1, typename T2, size_t R, size_t C>
		requires std::is_convertible_v<T2, T1>
	[[nodiscard]] constexpr matrix<R, C, T1>
	matrix_cast(const matrix<R, C, T2>& mat)
	{
		matrix<R, C, T1> temporary{};

		for (size_t r = 0; r < R; ++r)
			for (size_t c = 0; c < C; ++c)
				if constexpr (std::integral<T1> && std::floating_point<T2>)
					temporary(r, c) = static_cast<T1>(std::llround(mat(r, c)));
				else
					temporary(r, c) = static_cast<T1>(mat(r, c));

		return temporary;
	}

	template <typename V = void>
	struct MatrixTranspose
	{
		template <size_t R, size_t C, typename T>
		[[nodiscard]] constexpr auto operator()(const matrix<R, C, T>& m)
		{
				matrix<C, R, T> temporary{};

				for (size_t r = 0; r < R; ++r)
						for (size_t c = 0; c < C; ++c)
								temporary(c, r) = m(r, c);

				return temporary;
		}
	};

	template <size_t R, size_t C, typename T>
	[[nodiscard]] constexpr matrix<C, R, T>
	matrix_transpose(const matrix<R, C, T>& m)
	{
			return MatrixTranspose{}(m);
	}

	template <typename V = void>
	struct Submatrix
	{
		template <size_t R, size_t C, typename T> requires(R>=2 && C >= 2)
		[[nodiscard]] constexpr auto operator()(const matrix<R, C, T>& mat, const size_t row, const size_t col) -> matrix<R - 1, C - 1, T>
		{
			if (row >= R || col >= C)
				throw std::invalid_argument("row OR col out of range");

			matrix<R - 1, C - 1, T> temporary{};
			size_t r_relative = 0;

			for (size_t r = 0; r < R; ++r)
			{
				if (r == row)
				{
					r_relative = 1;
					continue;
				}

				size_t c_relative = 0;

				for (size_t c = 0; c < C; ++c)
				{
					if (c == col)
					{
						c_relative = 1;
						continue;
					}

					temporary(r - r_relative, c - c_relative) = mat(r, c);
				}
			}

			return temporary;
		}
	};

	template <size_t R, size_t C, typename T>
		requires(R >= 2 && C >= 2)
	[[nodiscard]] constexpr matrix<R - 1, C - 1, T>
	submatrix(const matrix<R, C, T>& mat, const size_t row, const size_t col)
	{
		return Submatrix{}(mat, row, col);
	}

	template <typename T>
	[[nodiscard]] constexpr T matrix_determinant(const matrix<2, 2, T>&);

	template <size_t E, typename T>
	[[nodiscard]] constexpr T matrix_determinant(const matrix<E, E, T>&);

	template <typename V = void>
	struct MatrixMinor
	{
		template <size_t E, typename T>
		[[nodiscard]] constexpr T operator()(const matrix<E, E, T>& mat, const size_t row, const size_t col)
		{
			if (row >= E || col >= E)
				throw std::invalid_argument("row OR col out of range");

			const auto temporary_submatrix = submatrix(mat, row, col);

			return matrix_determinant(temporary_submatrix);
		}
	};

	template <size_t E, typename T>
	[[nodiscard]] constexpr T matrix_minor(const matrix<E, E, T>& mat,
	                                       const size_t row, const size_t col)
	{
		return MatrixMinor{}(mat, row, col);
	}

	template <typename V = void>
	struct MatrixCofactor
	{
		template <size_t E, typename T>
		[[nodiscard]] constexpr T operator()(const matrix<E, E, T>& mat, const size_t row, const size_t col)
		{
			if (row >= E || col >= E)
				throw std::invalid_argument("row OR col out of range");

			const T minor = matrix_minor(mat, row, col);
			return (row + col) % 2 ? -minor : minor;
		}
	};

	template <size_t E, typename T>
	[[nodiscard]] constexpr T matrix_cofactor(const matrix<E, E, T>& mat,
	                                          const size_t row, const size_t col)
	{
		return MatrixCofactor{}(mat, row, col);
	}

	template <typename V = void>
	struct MatrixDeterminant
	{
		template <typename T>
		[[nodiscard]] constexpr T operator()(const matrix<2, 2, T>& mat)
		{
			return mat(0, 0) * mat(1, 1) - mat(0, 1) * mat(1, 0);
		}

		template <size_t E, typename T>
		[[nodiscard]] constexpr T operator()(const matrix<E, E, T>& mat)
		{
			T determinant{};

			for (size_t c = 0; c < E; ++c)
				determinant += mat(0, c) * matrix_cofactor(mat, 0, c);

			return determinant;
		}
	};

	template <typename T>
	[[nodiscard]] constexpr T matrix_determinant(const matrix<2, 2, T>& mat)
	{
		return MatrixDeterminant{}(mat);
	}

	template <size_t E, typename T>
	[[nodiscard]] constexpr T matrix_determinant(const matrix<E, E, T>& mat)
	{
		return MatrixDeterminant{}(mat);
	}

	// Maybe make functors of these functions to enable expected<...>(...) unit
	// tests to work with these

	template <typename V = void>
	struct InvertibleMatrix
	{
		template <size_t E, typename T>
		[[nodiscard]] constexpr bool operator()(const matrix<E, E, T>& mat)
		{
			return matrix_determinant(mat) != 0;
		}
	};

	template <size_t E, typename T>
	[[nodiscard]] constexpr bool is_invertible_matrix(const matrix<E, E, T>& mat)
	{
		return InvertibleMatrix{}(mat);
	}

	template <typename V = void>
	struct MatrixInverse
	{
		template <size_t E, typename T>
		[[nodiscard]] constexpr auto operator()(const matrix<E, E, T>& mat) -> matrix<E, E, std::conditional_t<std::floating_point<T>, T, long double>>
		{
			using RT = std::conditional_t<std::floating_point<T>, T, long double>;

			const RT determinant = matrix_determinant(mat);

			if constexpr (std::floating_point<T>)
			{
				if (are_close(determinant, T{}))
					throw std::domain_error("Matrix inversion undefined for singular zero "
						"value determinant matrix");
			}
			else
			{
				if (determinant == T{})
					throw std::domain_error("Matrix inversion undefined for singular zero "
						"value determinant matrix");
			}

			matrix<E, E, RT> transposed{};

			for (size_t r = 0; r < E; ++r)
			{
				for (size_t c = 0; c < E; ++c)
				{
					transposed(c, r) = matrix_cofactor(mat, r, c) / determinant;
					// doing (c, r) is intentional transpose step
				}
			}

			return transposed;
		}
	};

	template <size_t E, typename T>
	[[nodiscard]] constexpr matrix<
		E, E, std::conditional_t<std::floating_point<T>, T, long double>>
	matrix_inverse(const matrix<E, E, T>& mat)
	{
		return MatrixInverse{}(mat);
	}

	template <typename V = void>
	struct MatrixTranslate
	{
		template <typename T = long double>
		[[nodiscard]] constexpr matrix<4, 4, T> operator()(const vec<3, T>& vec)
		{
			auto temporary{ identity_matrix<4, T>() };

			for (size_t i = 0; i < 3; ++i)
			{
				temporary(i, 3) = vec[i];
			}

			return temporary;
		}
	};

	template <typename T = long double>
	[[nodiscard]] constexpr matrix<4, 4, T> matrix_translate(const vec<3, T>& vec)
	{
		return MatrixTranslate{}(vec);
	}

	template <typename V = void>
	struct MatrixScale
	{
		template <typename T = long double>
		[[nodiscard]] constexpr matrix<4, 4, T> operator()(const vec<3, T>& vec)
		{
			auto temporary{ identity_matrix<4, T>() };

			for (size_t i = 0; i < 3; ++i)
			{
				temporary(i, i) = vec[i];
			}

			return temporary;
		}
	};

	template <typename T = long double>
	[[nodiscard]] constexpr matrix<4, 4, T> matrix_scale(const vec<3, T>& vec)
	{
		return MatrixScale{}(vec);
	}

	template <typename V = void>
	struct MatrixShear
	{
		template <typename T = long double>
		[[nodiscard]] constexpr matrix<4, 4, T> operator()(const T x_y, const T x_z = {}, const T y_x = {}, const T y_z = {},
			const T z_x = {}, const T z_y = {})
		{
			return matrix<4, 4, T>{
				1, x_y, x_z, 0, y_x, 1, y_z, 0,
					z_x, z_y, 1, 0, 0, 0, 0, 1
			};
		}
	};

	template <typename T = long double>
	[[nodiscard]] constexpr matrix<4, 4, T>
	matrix_shear(const T x_y, const T x_z = {}, const T y_x = {}, const T y_z = {},
	             const T z_x = {}, const T z_y = {})
	{
		return MatrixShear{}(x_y, x_z, y_x, y_z, z_x, z_y);
	}

	template <typename V = void>
	struct MatrixRotateX
	{
		template <typename T = long double>
		[[nodiscard]] constexpr matrix<4, 4, T> operator()(const long double rad)
		{
			return matrix<4, 4, T>{
				1, 0, 0, 0, 0, c_cos(rad),
					-c_sin(rad), 0, 0, c_sin(rad), c_cos(rad), 0,
					0, 0, 0, 1
			};
		}
	};

	template <typename T = long double>
	[[nodiscard]] constexpr matrix<4, 4, T> matrix_rotate_x(const long double rad)
	{
		return MatrixRotateX{}(rad);
	}

	template <typename V = void>
	struct MatrixRotateY
	{
		template <typename T = long double>
		[[nodiscard]] constexpr matrix<4, 4, T> operator()(const long double rad)
		{
			return matrix<4, 4, T>{
				c_cos(rad), 0, c_sin(rad), 0, 0, 1, 0, 0,
					-c_sin(rad), 0, c_cos(rad), 0, 0, 0, 0, 1
			};
		}
	};
	
	template <typename T = long double>
	[[nodiscard]] constexpr matrix<4, 4, T> matrix_rotate_y(const long double rad)
	{
		return MatrixRotateY{}(rad);
	}

	template <typename V = void>
	struct MatrixRotateZ
	{
		template <typename T = long double>
		[[nodiscard]] constexpr matrix<4, 4, T> operator()(const long double rad)
		{
			return matrix<4, 4, T>{
				c_cos(rad), -c_sin(rad), 0, 0, c_sin(rad), c_cos(rad),
					0, 0, 0, 0, 1, 0,
					0, 0, 0, 1
			};
		}
	};

	template <typename T = long double>
	[[nodiscard]] constexpr matrix<4, 4, T> matrix_rotate_z(const long double rad)
	{
		return MatrixRotateZ{}(rad);
	}
} // namespace rtm

#endif
