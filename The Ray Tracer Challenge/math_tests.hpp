#ifndef MATH_TESTS_HPP
#define MATH_TESTS_HPP
#include "scene_object_tests.hpp"

namespace rtm::testing
{
	template <size_t R, size_t C, typename T>
		requires(R >= 2 && C >= 2)
	constexpr auto submatrix_helper(const size_t row, const size_t col)
	{
		return [&](const matrix<R, C, T>& mat) {return submatrix(mat, row, col); };
	}

	template <size_t E, typename T>
	constexpr auto matrix_minor_helper(const size_t row, const size_t col)
	{
		return [&](const matrix<E, E, T>& mat) {return matrix_minor(mat, row, col); };
	}

	template <size_t E, typename T>
	constexpr auto matrix_cofactor_helper(const size_t row, const size_t col)
	{
		return [&](const matrix<E, E, T>& mat) {return matrix_cofactor(mat, row, col); };
	}

	inline void perform_math_tests()
	{
		// point + vector = point
		expected(std::plus{}, rtm::vec<4, int>{1, 1, 6, 1}, rtm::vec<4, int>{3, -2, 5, 1},
		         rtm::vec<4, int>{-2, 3, 1, 0});

		// point - point = vector
		expected(std::minus{}, rtm::vec<4, int>{-2, -4, -6, 0}, rtm::vec<4, int>{3, 2, 1, 1},
		         rtm::vec<4, int>{5, 6, 7, 1});

		// point - vector = point
		expected(std::minus{}, rtm::vec<4, int>{-2, -4, -6, 1}, rtm::vec<4, int>{3, 2, 1, 1},
		         rtm::vec<4, int>{5, 6, 7, 0});

		// vector - vector = vector
		expected(std::minus{}, rtm::vec<4, int>{-2, -4, -6, 0}, rtm::vec<4, int>{3, 2, 1, 0},
		         rtm::vec<4, int>{5, 6, 7, 0});

		// -vector = vector
		expected(std::negate{}, rtm::vec<4, int>{-1, 2, -3, 0}, rtm::vec<4, int>{1, -2, 3, 0});

		// points here do not make sense and should not be multiplied or divided by
		// scalars vector * scalar = vector
		expected(std::multiplies{}, rtm::vec<4, double>{3.5, -7, 10.5, 0}, rtm::vec<4, double>{1, -2, 3, 0}, 3.5);

		// vector * scalar = vector
		expected(std::multiplies{}, rtm::vec<4, double>{.5, -1, 1.5, 0}, rtm::vec<4, double>{1, -2, 3, 0}, .5);

		// vector / scalar = vector
		// expected(std::divides{}, rtm::vec<4, double>{1, -2, 3, 0}, 2, rtm::vec<4,
		// double>{.5, -1, 1.5, 0});

		expected(Magnitude{}, 1, rtm::vec<4, int>{1, 0, 0, 0});

		expected(Magnitude{}, 1, rtm::vec<4, int>{0, 1, 0, 0});

		expected(Magnitude{}, 1, rtm::vec<4, int>{0, 0, 1, 0});

		expected(Magnitude{}, c_sqrt(14.), rtm::vec<4, int>{1, 2, 3, 0});

		expected(Magnitude{}, c_sqrt(14.), rtm::vec<4, int>{-1, -2, -3, 0});

		// vectors with a magnitude of 1 are UNIT VECTORS
		// we can turn a vector into a unit vector using NORMALIZATION
		expected(Normalize{}, rtm::vec<4, long double>{1, 0, 0, 0}, rtm::vec<4, int>{4, 0, 0, 0});

		expected(Normalize{}, rtm::vec<4, long double>{0.267261, 0.534522, 0.801784, 0.}, rtm::vec<4, int>{1, 2, 3, 0});

		expected(Magnitude{}, 1, normalize(rtm::vec<4, int>{1, 2, 3, 0}));

		expected(DotProduct{}, 20, rtm::vec<4, int>{1, 2, 3, 0},
		         rtm::vec<4, int>{2, 3, 4, 0});

		expected(CrossProduct{}, rtm::vec<4, int>{-1, 2, -1, 0}, rtm::vec<4, int>{1, 2, 3, 0},
		         rtm::vec<4, int>{2, 3, 4, 0});

		expected(CrossProduct{}, rtm::vec<4, int>{1, -2, 1, 0}, rtm::vec<4, int>{2, 3, 4, 0},
		         rtm::vec<4, int>{1, 2, 3, 0});

		expected(std::plus{}, rtm::vec<4, float>{1.6f, 0.7f, 1.0f, 0.0f}, rtm::vec<4, float>{0.9f, 0.6f, 0.75f, 0.0f},
		         rtm::vec<4, float>{0.7f, 0.1f, 0.25f, 0.0f});

		expected(std::minus{}, rtm::vec<4, float>{0.2f, 0.5f, 0.5f, 0.0f}, rtm::vec<4, float>{0.9f, 0.6f, 0.75f, 0.0f},
		         rtm::vec<4, float>{0.7f, 0.1f, 0.25f, 0.0f});

		expected(std::multiplies{}, rtm::vec<4, float>{0.4f, 0.6f, 0.8f, 0.0f}, rtm::vec<4, float>{0.2f, 0.3f, 0.4f, 0.0f}, 2.f);

		expected(std::multiplies{}, rtm::vec<4, float>{0.9f, 0.2f, 0.04f, 0.0f}, rtm::vec<4, float>{1.0f, 0.2f, 0.4f, 0.0f},
		         rtm::vec<4, float>{0.9f, 1.0f, 0.1f, 0.0f});

		expected(InvertibleMatrix{}, true, rtm::matrix<4, 4, int>{6, 4, 4, 4, 5, 5, 7, 6, 4, -9, 3, -7, 9, 1, 7, -6});

		/*expected(
			static_cast<bool (*)(const matrix<4, 4, int>&)>(is_invertible_matrix),
			rtm::matrix<4, 4, int>{6, 4, 4, 4, 5, 5, 7, 6, 4, -9, 3, -7, 9, 1, 7, -6},
			true);*/

		expected(InvertibleMatrix{}, false, rtm::matrix<4, 4, int>{
			         -4, 2, -2, -3, 9, 6, 2, 6, 0, -5, 1, -5, 0, 0, 0,
			         0});

		/*expected(
			static_cast<bool (*)(const matrix<4, 4, int>&)>(is_invertible_matrix),
			rtm::matrix<4, 4, int>{
				-4, 2, -2, -3, 9, 6, 2, 6, 0, -5, 1, -5, 0, 0, 0,
				0
			},
			false);*/

		expected(rtm::matrix<3, 3, int>{1, 0, 0, 0, 1, 0, 0, 0, 1}, identity_matrix<3, int>());

		/*expected(rtm::matrix<3, 3, int>{1, 0, 0, 0, 1, 0, 0, 0, 1},
		         rtm::identity_matrix<3, int>());*/

		expected(rtm::matrix<4, 4, int>{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1}, identity_matrix<4, int>());

		/*expected(
			rtm::matrix<4, 4, int>{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1},
			rtm::identity_matrix<4, int>());*/

		expected(
			std::multiplies{}, rtm::vec<4, int>{18, 24, 33, 1}, 
			rtm::matrix<4, 4, int>{1, 2, 3, 4, 2, 4, 4, 2, 8, 6, 4, 1, 0, 0, 0, 1},
			rtm::vec<4, int>{1, 2, 3, 1});

		expected(MatrixTranspose{}, rtm::matrix<4, 4, int>{0, 9, 1, 0, 9, 8, 8, 0, 3, 0, 5, 5, 0, 8, 3, 8}, rtm::matrix<4, 4, int>{0, 9, 3, 0, 9, 8, 0, 8, 1, 8, 5, 3, 0, 0, 5, 8});

		/*expected(
			static_cast<matrix<4, 4, int> (*)(const matrix<4, 4, int>&)>(
				matrix_transpose),
			rtm::matrix<4, 4, int>{0, 9, 3, 0, 9, 8, 0, 8, 1, 8, 5, 3, 0, 0, 5, 8},
			rtm::matrix<4, 4, int>{0, 9, 1, 0, 9, 8, 8, 0, 3, 0, 5, 5, 0, 8, 3, 8});*/

		expected(MatrixTranspose{}, identity_matrix<4, int>(), identity_matrix<4, int>());

		/*expected(static_cast<matrix<4, 4, int> (*)(const matrix<4, 4, int>&)>(
			         matrix_transpose),
		         rtm::identity_matrix<4, int>(), rtm::identity_matrix<4, int>());*/

		expected(MatrixDeterminant{}, 17, rtm::matrix<2, 2, int>{1, 5, -3, 2});

		/*expected(static_cast<int (*)(const matrix<2, 2, int>&)>(matrix_determinant),
		         rtm::matrix<2, 2, int>{1, 5, -3, 2}, 17);*/

		expected(Submatrix{}, rtm::matrix<2, 2, int>{-3, 2, 0, 6}, rtm::matrix<3, 3, int>{1, 5, 0, -3, 2, 7, 0, 6, -3}, 0, 2);

		/*{
			auto subm_test_result1 =
				submatrix(rtm::matrix<3, 3, int>{1, 5, 0, -3, 2, 7, 0, 6, -3}, 0, 2);

			expected(subm_test_result1, rtm::matrix<2, 2, int>{-3, 2, 0, 6});
		}*/

		expected(Submatrix{}, rtm::matrix<3, 3, int>{-6, 1, 6, -8, 8, 6, -7, -1, 1}, rtm::matrix<4, 4, int>{
			-6, 1, 1, 6, -8, 5, 8, 6, -1, 0, 8, 2,
				-7, 1, -1, 1
		}, 2, 1);

		/*{
			auto subm_test_result2 =
				submatrix(rtm::matrix<4, 4, int>{
					          -6, 1, 1, 6, -8, 5, 8, 6, -1, 0, 8, 2,
					          -7, 1, -1, 1
				          },
				          2, 1);

			expected(subm_test_result2,
			         rtm::matrix<3, 3, int>{-6, 1, 6, -8, 8, 6, -7, -1, 1});
		}*/

		expected(MatrixMinor{}, 25, matrix<3, 3, int>{3, 5, 0, 2, -1, -7, 6, -1, 5}, 1, 0);

		expected(MatrixCofactor{}, -12, matrix<3, 3, int>{3, 5, 0, 2, -1, -7, 6, -1, 5}, 0, 0);

		expected(MatrixCofactor{}, -25, matrix<3, 3, int>{3, 5, 0, 2, -1, -7, 6, -1, 5}, 1, 0);

		/*{
			auto m1 = rtm::matrix<3, 3, int>{3, 5, 0, 2, -1, -7, 6, -1, 5};
			{
				auto minor_test_result1 = matrix_minor(m1, 1, 0);

				expected(minor_test_result1, 25);
			}

			expected(matrix_cofactor(m1, 0, 0), -12);

			expected(matrix_cofactor(m1, 1, 0), -25);
		}*/

		expected(MatrixDeterminant{}, -196, matrix<3, 3, int>{1, 2, 6, -5, 8, -4, 2, 6, 4});

		/*expected(
			matrix_determinant(rtm::matrix<3, 3, int>{1, 2, 6, -5, 8, -4, 2, 6, 4}),
			-196);*/

		expected(MatrixDeterminant{}, -4071, matrix<4, 4, int>{
			-2, -8, 3, 5, -3, 1, 7, 3, 1, 2, -9, 6, -6, 7, 7, -9
		});

		/*expected(matrix_determinant(rtm::matrix<4, 4, int>{
			         -2, -8, 3, 5, -3, 1, 7, 3, 1, 2, -9, 6, -6, 7, 7, -9
		         }),
		         -4071);*/

		expected(InvertibleMatrix{}, true, matrix<4, 4, int>{
			6, 4, 4, 4, 5, 5, 7, 6, 4, -9, 3, -7, 9, 1, 7, -6
		});

		/*expected(is_invertible_matrix(rtm::matrix<4, 4, int>{
			         6, 4, 4, 4, 5, 5, 7, 6, 4, -9, 3, -7, 9, 1, 7, -6
		         }),
		         true);*/

		expected(InvertibleMatrix{}, false, matrix<4, 4, int>{
			-4, 2, -2, -3, 9, 6, 2, 6, 0, -5, 1, -5, 0, 0, 0, 0
		});

		/*expected(is_invertible_matrix(rtm::matrix<4, 4, int>{
			         -4, 2, -2, -3, 9, 6, 2, 6, 0, -5, 1, -5, 0, 0, 0, 0
		         }),
		         false);*/

		expected(MatrixInverse{}, matrix<4, 4, long double>{
			-0.15384615, -0.15384615, -0.28205128, -0.53846154, -0.076923077,
				0.12307692, 0.025641026, 0.030769231, 0.35897436, 0.35897436,
				0.43589744, 0.92307692, -0.69230769, -0.69230769, -0.76923077,
				-1.9230769
		}, matrix<4, 4, long double>{
			8, -5, 9, 2, 7, 5, 6, 1, -6, 0, 9, 6,
				-3, 0, -9, -4
		});

		/*expected(static_cast<matrix<4, 4, long double> (*)(
			         const matrix<4, 4, long double>&)>(matrix_inverse),
		         rtm::matrix<4, 4, long double>{
			         8, -5, 9, 2, 7, 5, 6, 1, -6, 0, 9, 6,
			         -3, 0, -9, -4
		         },
		         rtm::matrix<4, 4, long double>{
			         -0.15384615, -0.15384615, -0.28205128, -0.53846154, -0.076923077,
			         0.12307692, 0.025641026, 0.030769231, 0.35897436, 0.35897436,
			         0.43589744, 0.92307692, -0.69230769, -0.69230769, -0.76923077,
			         -1.9230769
		         });*/

		expected(MatrixInverse{}, matrix<4, 4, long double>{
			-0.040740741, -0.077777778, 0.14444444, -0.22222222,
				-0.077777778, 0.033333333, 0.36666667, -0.33333333, -0.029012346,
				-0.1462963, -0.10925926, 0.12962963, 0.17777778, 0.066666667,
				-0.26666667, 0.33333333
		}, matrix<4, 4, long double>{
			9, 3, 0, 9, -5, -2, -6, -3, -4, 9, 6,
				4, -7, 6, 6, 2
		});

		/*expected(static_cast<matrix<4, 4, long double> (*)(
			         const matrix<4, 4, long double>&)>(matrix_inverse),
		         rtm::matrix<4, 4, long double>{
			         9, 3, 0, 9, -5, -2, -6, -3, -4, 9, 6,
			         4, -7, 6, 6, 2
		         },
		         rtm::matrix<4, 4, long double>{
			         -0.040740741, -0.077777778, 0.14444444, -0.22222222,
			         -0.077777778, 0.033333333, 0.36666667, -0.33333333, -0.029012346,
			         -0.1462963, -0.10925926, 0.12962963, 0.17777778, 0.066666667,
			         -0.26666667, 0.33333333
		         });*/

		expected(std::multiplies{}, vec<4, int>{2, 1, 7, 1}, matrix_translate<int>({ 5, -3, 2 }), vec<4, int>{-3, 4, 5, 1});

		expected(std::multiplies{}, vec<4, int>{-8, 7, 3, 1}, matrix_cast<int>(matrix_inverse(matrix_translate<int>({ 5, -3, 2 }))), vec<4, int>{-3, 4, 5, 1});

		expected(std::multiplies{}, vec<4, int>{-3, 4, 5, 0}, matrix_translate<int>({ 5, -3, 2 }), vec<4, int>{-3, 4, 5, 0});

		//{
		//	auto transform = matrix_translate<int>({5, -3, 2});

		//	auto inverse = matrix_inverse(transform);

		//	expected(vec<4, int>{2, 1, 7, 1}, transform * vec<4, int>{-3, 4, 5, 1});

		//	expected(vec<4, int>{-8, 7, 3, 1},
		//	         matrix_cast<int>(inverse) * vec<4, int>{-3, 4, 5, 1});

		//	// Vectors aren't affected, because w = 0
		//	expected(vec<4, int>{-3, 4, 5, 0}, transform * vec<4, int>{-3, 4, 5, 0});
		//}

		expected(std::multiplies{}, vec<4, int>{-8, 18, 32, 1}, matrix_scale<int>({2, 3, 4}), vec<4, int>{-4, 6, 8, 1});

		expected(std::multiplies{}, vec<4, int>{-8, 18, 32, 0}, matrix_scale<int>({2, 3, 4}), vec<4, int>{-4, 6, 8, 0});

		expected(std::multiplies{}, vec<4, long double>{-2, 2, 2, 0}, matrix_inverse(matrix_scale<int>({ 2, 3, 4 })), vec<4, long double>{-4, 6, 8, 0});

		/*{
			auto transform = matrix_scale<int>({2, 3, 4});

			expected(vec<4, int>{-8, 18, 32, 1}, transform * vec<4, int>{-4, 6, 8, 1});

			expected(vec<4, int>{-8, 18, 32, 0}, transform * vec<4, int>{-4, 6, 8, 0});

			auto inverse = matrix_inverse(transform);
			expected(vec<4, long double>{-2, 2, 2, 0},
			         inverse * vec<4, long double>{-4, 6, 8, 0});
		}*/

		expected(std::multiplies{}, vec<4, long double>{0, c_sqrt(2.L) / 2, c_sqrt(2.L) / 2, 1}, matrix_rotate_x<long double>(constants::PI / 4), vec<4, long double>{0, 1, 0, 1});

		expected(std::multiplies{}, vec<4, long double>{0, 0, 1, 1}, matrix_rotate_x<long double>(constants::PI / 2), vec<4, long double>{0, 1, 0, 1});

		expected(std::multiplies{}, vec<4, long double>{0, c_sqrt(2.L) / 2, -c_sqrt(2.L) / 2, 1}, matrix_inverse(matrix_rotate_x<long double>(constants::PI / 4)), vec<4, long double>{0, 1, 0, 1});

		/*{
			auto half_quarter = matrix_rotate_x<long double>(constants::PI / 4);
			auto inverse = matrix_inverse(half_quarter);
			auto full_quarter = matrix_rotate_x<long double>(constants::PI / 2);

			auto point = vec<4, long double>{0, 1, 0, 1};

			expected(vec<4, long double>{0, c_sqrt(2.L) / 2, c_sqrt(2.L) / 2, 1},
			         half_quarter * point);

			expected(vec<4, long double>{0, 0, 1, 1}, full_quarter * point);

			expected(vec<4, long double>{0, c_sqrt(2.L) / 2, -c_sqrt(2.L) / 2, 1},
			         inverse * point);
		}*/

		expected(std::multiplies{}, vec<4, long double>{c_sqrt(2.L) / 2, 0, c_sqrt(2.L) / 2, 1}, matrix_rotate_y<long double>(constants::PI / 4), vec<4, long double>{0, 0, 1, 1});

		expected(std::multiplies{}, vec<4, long double>{1, 0, 0, 1}, matrix_rotate_y<long double>(constants::PI / 2), vec<4, long double>{0, 0, 1, 1});


		/*{
			auto half_quarter = matrix_rotate_y<long double>(constants::PI / 4);
			auto full_quarter = matrix_rotate_y<long double>(constants::PI / 2);

			auto point = vec<4, long double>{0, 0, 1, 1};

			expected(vec<4, long double>{c_sqrt(2.L) / 2, 0, c_sqrt(2.L) / 2, 1},
			         half_quarter * point);

			expected(vec<4, long double>{1, 0, 0, 1}, full_quarter * point);
		}*/

		expected(std::multiplies{}, vec<4, long double>{-c_sqrt(2.L) / 2, c_sqrt(2.L) / 2, 0, 1}, matrix_rotate_z<long double>(constants::PI / 4), vec<4, long double>{0, 1, 0, 1});

		expected(std::multiplies{}, vec<4, long double>{-1, 0, 0, 1}, matrix_rotate_z<long double>(constants::PI / 2), vec<4, long double>{0, 1, 0, 1});

		/*{
			auto half_quarter = matrix_rotate_z<long double>(constants::PI / 4);
			auto full_quarter = matrix_rotate_z<long double>(constants::PI / 2);

			auto point = vec<4, long double>{0, 1, 0, 1};

			expected(vec<4, long double>{-c_sqrt(2.L) / 2, c_sqrt(2.L) / 2, 0, 1},
			         half_quarter * point);

			expected(vec<4, long double>{-1, 0, 0, 1}, full_quarter * point);
		}*/

		{
			auto x_y = matrix_shear<long double>(1, 0, 0, 0, 0, 0);
			auto x_z = matrix_shear<long double>(0, 1, 0, 0, 0, 0);
			auto y_x = matrix_shear<long double>(0, 0, 1, 0, 0, 0);
			auto y_z = matrix_shear<long double>(0, 0, 0, 1, 0, 0);
			auto z_x = matrix_shear<long double>(0, 0, 0, 0, 1, 0);
			auto z_y = matrix_shear<long double>(0, 0, 0, 0, 0, 1);

			auto point = vec<4, long double>{ 2, 3, 4, 1 };

			expected(std::multiplies{}, vec<4, long double>{5, 3, 4, 1}, x_y, point);
			expected(std::multiplies{}, vec<4, long double>{6, 3, 4, 1}, x_z, point);
			expected(std::multiplies{}, vec<4, long double>{2, 5, 4, 1}, y_x, point);
			expected(std::multiplies{}, vec<4, long double>{2, 7, 4, 1}, y_z, point);
			expected(std::multiplies{}, vec<4, long double>{2, 3, 6, 1}, z_x, point);
			expected(std::multiplies{}, vec<4, long double>{2, 3, 7, 1}, z_y, point);
		}

		/*{
			auto x_y = matrix_shear<long double>(1, 0, 0, 0, 0, 0);
			auto x_z = matrix_shear<long double>(0, 1, 0, 0, 0, 0);
			auto y_x = matrix_shear<long double>(0, 0, 1, 0, 0, 0);
			auto y_z = matrix_shear<long double>(0, 0, 0, 1, 0, 0);
			auto z_x = matrix_shear<long double>(0, 0, 0, 0, 1, 0);
			auto z_y = matrix_shear<long double>(0, 0, 0, 0, 0, 1);

			auto point = vec<4, long double>{2, 3, 4, 1};

			expected(vec<4, long double>{5, 3, 4, 1}, x_y * point);
			expected(vec<4, long double>{6, 3, 4, 1}, x_z * point);
			expected(vec<4, long double>{2, 5, 4, 1}, y_x * point);
			expected(vec<4, long double>{2, 7, 4, 1}, y_z * point);
			expected(vec<4, long double>{2, 3, 6, 1}, z_x * point);
			expected(vec<4, long double>{2, 3, 7, 1}, z_y * point);
		}*/

		{
			constexpr auto A = matrix_rotate_x(constants::PI / 2);
			constexpr auto B = matrix_scale<long double>({ 5, 5, 5 });
			constexpr auto C = matrix_translate<long double>({ 10, 5, 7 });

			expected(std::multiplies{}, vec<4, long double>{1, -1, 0, 1}, A, vec<4, long double>{1, 0, 1, 1});
			expected(std::multiplies{}, vec<4, long double>{5, -5, 0, 1}, B, A * vec<4, long double>{1, 0, 1, 1});
			expected(std::multiplies{}, vec<4, long double>{15, 0, 7, 1}, C, B * A * vec<4, long double>{1, 0, 1, 1});
			expected(C * B * A * vec<4, long double>{1, 0, 1, 1}, vec<4, long double>{15, 0, 7, 1});
		}

		//{
		//	auto point_1 = vec<4, long double>{1, 0, 1, 1};

		//	auto A = matrix_rotate_x(constants::PI / 2);
		//	auto B = matrix_scale<long double>({5, 5, 5});
		//	auto C = matrix_translate<long double>({10, 5, 7});

		//	auto point_2 = A * point_1;

		//	expected(vec<4, long double>{1, -1, 0, 1}, point_2);

		//	auto point_3 = B * point_2;

		//	expected(vec<4, long double>{5, -5, 0, 1}, point_3);

		//	auto point_4 = C * point_3;

		//	expected(vec<4, long double>{15, 0, 7, 1}, point_4);

		//	// Associative property
		//	auto D = C * B * A;

		//	// Commutative property isn't guaranteed for matrices

		//	expected(point_4, D * point_1);
		//}
	}
} // namespace rtm::testing

#endif
