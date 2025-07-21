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

    // Bolinhas aleatórias!
    double random_balls_size = 0.08;
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), random_balls_size, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, random_balls_size, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, random_balls_size, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, random_balls_size, sphere_material));
                }
            }
        }
    }

    // Coração!
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
    }

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 960;
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
