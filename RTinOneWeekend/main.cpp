#include "rtweekend.h"

#include "camera.h"
#include "colour.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"


#include <iostream>

colour ray_colour(const ray& r, const hittable& world, int depth = 1)
{
	hit_record rec;

	// If we've exceeded the ray bounce limit, no more light is gathered.
	if (depth <= 0)
		return color(0, 0, 0);

	if (world.hit(r, 0.001, infinity, rec))
	{
		ray scattered;
		colour attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			return attenuation * ray_colour(scattered, world, depth - 1);
		}
		return colour(0, 0, 0);
	}

	vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5 * (unit_direction.y() + 1.0);
	return (1.0 - t) * colour(1.0, 1.0, 1.0) + t * colour(0.5, 0.7, 1.0);
}


int main()
{
	// Image
	const auto aspect_ratio = 16.0 / 9.0;
	const int image_width = 400;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int sample_per_pixel = 100;
	const int max_depth = 50;

	// World
	hittable_list world;

	// - Materials
	auto material_ground = make_shared<lambertian>(colour(0.8, 0.8, 0.));
	auto material_centre = make_shared<lambertian>(colour(0.1, 0.2, 0.5));
	auto material_left = make_shared<dielectric>(1.5);
	auto material_right = make_shared<metal>(colour(0.8, 0.6, 0.2), 0.0);

	// - Add to world
	world.add(make_shared<sphere>(point3(0, -100.5, -1.), 100, material_ground));
	world.add(make_shared<sphere>(point3(0.0, 0.0, -1.), 0.5, material_centre));
	world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.), 0.5, material_left));
	world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.), -0.4, material_left));
	world.add(make_shared<sphere>(point3(1.0, 0.0, -1.), 0.5, material_right));
	

	// Camera
	camera cam;

	// Render

	std::cout << "P3\n" << image_width << ' ' << image_width << "\n255\n";

	for (int j = image_height - 1; j >= 0; --j)
	{
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
		for (int i = 0; i < image_width; ++i)
		{
			colour pixel_colour(0, 0, 0);

			for (int s = 0; s < sample_per_pixel; ++s)
			{
				auto u = (i + random_double()) / (image_width - 1);
				auto v = (j + random_double()) / (image_height - 1);
				ray r = cam.get_ray(u, v);
				pixel_colour += ray_colour(r, world, max_depth);
			}
			write_colour(std::cout, pixel_colour, sample_per_pixel);
		}
	}

	std::cerr << "\nDone.\n";
}