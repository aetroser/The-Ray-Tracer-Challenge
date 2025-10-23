#include "canvas.hpp"
#include "lighting.hpp"
#include "scene_object_tests.hpp" // Assuming this contains your math/scene classes
#include <iostream>
#include <memory>

// Define canvas dimensions in one place for clarity and easy modification
namespace
{
	constexpr int CANVAS_WIDTH = 3000;
	constexpr int CANVAS_HEIGHT = 3000;
} // namespace

// The render function is encapsulated for clean design.
// The canvas is passed by reference to be modified.
void render(rtm::canvas<CANVAS_WIDTH, CANVAS_HEIGHT>& scene)
{
	// Camera position in the world
	constexpr auto camera_origin = rtm::vec<4, long double>{0.0, 0.0, -1.5, 1};

	// A sphere at the origin
	auto sphere = rtm::sphere::make();
	sphere->properties.color = {1, 0.2, 1};

	rtm::point_light light_source{{1, 1, 1}, {-10, 10, -10, 1}};

	// sphere->set_transform(rtm::matrix_translate({ 1.0, 0.0, 0.0 }));

	// The Z position of the virtual "screen" or projection plane
	constexpr long double projection_plane_z = 500;

	// World coordinates corresponding to the top-left of the canvas
	constexpr auto world_min_x = -CANVAS_WIDTH / 2.0;
	constexpr auto world_min_y = -CANVAS_HEIGHT / 2.0;

	// Iterate over each pixel of the canvas using 0-based indexing
	for (int y = 0; y < CANVAS_HEIGHT; ++y) // y represents the row
	{
		for (int x = 0; x < CANVAS_WIDTH; ++x) // x represents the column
		{
			const auto world_x = world_min_x + x;
			const auto world_y = world_min_y + y;
			const auto point_on_plane =
				rtm::vec<4, long double>{world_x, world_y, projection_plane_z, 1.0};

			const auto ray_direction = rtm::normalize(point_on_plane - camera_origin);
			const rtm::ray<long double> ray = {camera_origin, ray_direction};

			auto hit = sphere->intersect(ray);

			if (hit.has_value())
			{
				auto point = rtm::position(ray, hit.value()[0].t);
				auto normal = rtm::normal_at(std::dynamic_pointer_cast<rtm::sphere>(
					                             hit.value()[0].object.lock()),
				                             point);
				auto eye = -ray.direction;

				const rtm::clr1 final_color_fp =
					rtm::lighting(sphere->properties, light_source, point, eye, normal);

				// Helper function for clamping might be useful
				auto clamp = [](long double value, long double min, long double max)
				{
					if (value < min)
						return min;
					if (value > max)
						return max;
					return value;
				};

				// 2. Scale, clamp, and cast each component
				uint8_t r =
					static_cast<uint8_t>(clamp(final_color_fp[0] * 255.0, 0.0, 255.0));
				uint8_t g =
					static_cast<uint8_t>(clamp(final_color_fp[1] * 255.0, 0.0, 255.0));
				uint8_t b =
					static_cast<uint8_t>(clamp(final_color_fp[2] * 255.0, 0.0, 255.0));

				scene(y, x) = rtm::clr255{r, g, b};
			}
		}
	}
}

int main()
{
	rtm::canvas<CANVAS_WIDTH, CANVAS_HEIGHT> scene{};

	render(scene);

	// rtm::testing::perform_tests();
	// rtm::testing::perform_scene_tests();

	// 91 strona lighting and shading

	return 0;
}
