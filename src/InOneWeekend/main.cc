//==============================================================================================
// Originally written in 2016 by Peter Shirley <ptrshrl@gmail.com>
//
// To the extent possible under law, the author(s) have dedicated all copyright and related and
// neighboring rights to this software to the public domain worldwide. This software is
// distributed without any warranty.
//
// You should have received a copy (see file COPYING.txt) of the CC0 Public Domain Dedication
// along with this software. If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
//==============================================================================================

#include "rtweekend.h"

#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"

#include <vector>
#include <utility>


int main() {
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.2, 0.3, 0.8));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    std::vector<std::pair<point3, double>> heart_sphere_data;

    int num_spheres = 17;
    double sphere_radius = num_spheres / 85.0;
    double scale_factor = 0.08;
    point3 heart_origin = point3(0, sphere_radius, 0);

    for (int i = 0; i < num_spheres; ++i) {
        if (i == num_spheres / 2) continue;
        if (i == 1) continue;
        if (i == num_spheres - 1) continue;

        double t = 2 * pi * i / num_spheres;

        double x = 16 * pow(sin(t), 3);
        double y = 13 * cos(t) - 5 * cos(2 * t) - 2 * cos(3 * t) - cos(4 * t);

        point3 center = heart_origin + vec3(x * scale_factor, 0, y * scale_factor);
        shared_ptr<material> sphere_material;

        int material_type = i % 3;

        if (material_type == 0) {
            sphere_material = make_shared<lambertian>(color(random_double(), random_double(), random_double()));
        } else if (material_type == 1) {
            sphere_material = make_shared<metal>(color(random_double(0.5, 1), random_double(0.5, 1), random_double(0.5, 1)), random_double(0, 0.05));
        } else {
            sphere_material = make_shared<dielectric>(1.5);
        }
        world.add(make_shared<sphere>(center, sphere_radius, sphere_material));
        heart_sphere_data.push_back({center, sphere_radius}); // Store heart sphere data
    }

    int num_additional_spheres_x = 10;
    int num_additional_spheres_z = 10;
    double spacing = 0.5;

    for (int i = 0; i < num_additional_spheres_x; ++i) {
        for (int j = 0; j < num_additional_spheres_z; ++j) {
            point3 center(
                -2.0 + i * spacing + random_double(-0.1, 0.1),
                0.1,
                -2.0 + j * spacing + random_double(-0.1, 0.1)
            );

            bool overlaps_with_heart = false;
            for (const auto& heart_s : heart_sphere_data) {
                double distance = (center - heart_s.first).length();
                if (distance < (0.1 + heart_s.second)) { // 0.1 is radius of small sphere
                    overlaps_with_heart = true;
                    break;
                }
            }

            if (overlaps_with_heart) {
                continue; // Skip this small sphere if it overlaps
            }

            shared_ptr<material> sphere_material;

            double choose_mat = random_double();
            if (choose_mat < 0.8) {
                // Diffuse
                sphere_material = make_shared<lambertian>(color(random_double(), random_double(), random_double()));
            } else if (choose_mat < 0.95) {
                // Metal
                sphere_material = make_shared<metal>(color(random_double(0.5, 1), random_double(0.5, 1), random_double(0.5, 1)), random_double(0, 0.05));
            } else {
                // Glass
                sphere_material = make_shared<dielectric>(1.5);
            }
            world.add(make_shared<sphere>(center, 0.1, sphere_material)); // Small radius for additional spheres
        }
    }

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 320;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 20;
    cam.lookfrom = point3(0, 4, -6);
    cam.lookat = point3(0, sphere_radius, 0);
    cam.vup = vec3(0,1,0);

    cam.defocus_angle = 0.0;
    cam.focus_dist = 10.0;

    cam.render(world);
}
