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


int main() {
    // World
    hittable_list world;

    // Ground
    auto ground_material = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

    double sphere_radius = 0.2;
    double scale_factor = 0.15; // Adjust to control the size of the heart
    point3 heart_origin = point3(0, 2, 0); // Center the heart higher up

    int num_spheres = 200; // More spheres for a smoother shape
    for (int i = 0; i < num_spheres; ++i) {
        double t = 2 * pi * i / num_spheres;

        // Parametric equation for a heart shape
        double x = 16 * pow(sin(t), 3);
        double y = 13 * cos(t) - 5 * cos(2 * t) - 2 * cos(3 * t) - cos(4 * t);

        point3 center = heart_origin + vec3(x * scale_factor, y * scale_factor, 0);
        shared_ptr<material> sphere_material;

        // Vary material based on loop index
        int material_type = i % 4; // Cycle through 4 material types

        if (material_type == 0) {
            // Diffuse (varying color)
            sphere_material = make_shared<lambertian>(color(random_double(), random_double(), random_double()));
        } else if (material_type == 1) {
            // Metal (varying color and fuzz)
            sphere_material = make_shared<metal>(color(random_double(0.5, 1), random_double(0.5, 1), random_double(0.5, 1)), random_double(0, 0.5));
        } else if (material_type == 2) {
            // Glass (fixed refractive index)
            sphere_material = make_shared<dielectric>(1.5);
        } else {
            // Another diffuse with a more pastel color range
            sphere_material = make_shared<lambertian>(color(random_double(0.7, 1), random_double(0.7, 1), random_double(0.7, 1)));
        }
        world.add(make_shared<sphere>(center, sphere_radius, sphere_material));
    }

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth = 50;

    cam.vfov = 20;
    cam.lookfrom = point3(0, 10, 5); // Higher and looking down
    cam.lookat = point3(0, 2, 0); // Look at the center of the heart
    cam.vup = vec3(0,1,0);

    cam.defocus_angle = 0.0;
    cam.focus_dist = 10.0;

    cam.render(world);
}
