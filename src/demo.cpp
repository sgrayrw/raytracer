/*
 * Adapted from materials.cpp
 */

#include "ppm_image.h"
#include "AGLM.h"
#include "ray.h"
#include "sphere.h"
#include "plane.h"
#include "camera.h"
#include "material.h"
#include "hittable_list.h"

using namespace glm;
using namespace agl;
using namespace std;

color ray_color(const ray &r, const hittable_list &world, int depth) {
    hit_record rec;
    if (depth <= 0) {
        return color(0);
    }

    if (world.hit(r, 0.001f, infinity, rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            color recurseColor = ray_color(scattered, world, depth - 1);
            return attenuation * recurseColor;
        }
        return attenuation;
    }
    vec3 unit_direction = normalize(r.direction());
    auto t = 0.5f * (unit_direction.y + 1.0f);
    return (1.0f - t) * color(1, 1, 1) + t * color(0.5f, 0.7f, 1.0f);
}

color normalize_color(const color &c, int samples_per_pixel) {
    float scale = 1.0f / samples_per_pixel;
    float r = std::min(0.999f, std::max(0.0f, c.r * scale));
    float g = std::min(0.999f, std::max(0.0f, c.g * scale));
    float b = std::min(0.999f, std::max(0.0f, c.b * scale));

    // apply gamma correction
    r = sqrt(r);
    g = sqrt(g);
    b = sqrt(b);

    return color(r, g, b);
}

void ray_trace(ppm_image &image) {
    // Image
    int height = image.height();
    int width = image.width();
    float aspect = width / float(height);
    int samples_per_pixel = 10; // higher => more anti-aliasing
    int max_depth = 10; // higher => less shadow acne

    // Camera
    vec3 camera_pos(0, 0, 6);
    float viewport_height = 2.0f;
    float focal_length = 4.0f;
    camera cam(camera_pos, viewport_height, aspect, focal_length);
    //camera cam(point3(-10,10,5), point3(0,0,-1), vec3(0,1,0), 80, aspect); // a different camera pos

    // World
    shared_ptr<material> matteGreen = make_shared<lambertian>(color(0, 0.5f, 0));
    shared_ptr<material> metalRed = make_shared<metal>(color(1, 0, 0), 0.3f);
    shared_ptr<material> glass = make_shared<dielectric>(1.5f);
    shared_ptr<material> phongDefault = make_shared<phong>(camera_pos);

    shared_ptr<material> background = make_shared<phong>(camera_pos, color(0), 100.f);
    shared_ptr<material> sun = make_shared<lambertian>(color(0.8, 0, 0));
    shared_ptr<material> mercury = make_shared<lambertian>(color(63.0f / 255, 61.0f / 255, 63.0f / 255));
    shared_ptr<material> venus = make_shared<lambertian>(color(131.f / 255, 75.f / 255, 22.f / 255));
    shared_ptr<material> earth = make_shared<phong>(camera_pos, color(30.f / 255, 50.f / 255, 255.f / 255), 10.f);
    shared_ptr<material> moon = make_shared<metal>(color(0.5f), 0.3f);
    shared_ptr<material> mars = make_shared<metal>(color(120.f / 255, 79.f / 255, 59.f / 255), 0.9f);
    shared_ptr<material> jupyter = make_shared<phong>(camera_pos, color(255.f / 255, 100.f / 255, 50.f / 255), 8.f);
    shared_ptr<material> saturn = make_shared<phong>(camera_pos, color(246.f / 255, 200.f / 255, 116.f / 255), 10.f);
    shared_ptr<material> uranus = make_shared<lambertian>(color(209.f / 255, 238.f / 255, 241.f / 255));

    hittable_list world;
    world.add(make_shared<plane>(point3(0, 0, -100), vec3(0, 0, 1), background));
    world.add(make_shared<sphere>(point3(-5.5, 0, -1), 3.0f, sun));
    world.add(make_shared<sphere>(point3(-2.1, 0, -1), 0.14f, mercury));
    world.add(make_shared<sphere>(point3(-1.7, 0, -1), 0.2f, venus));
    world.add(make_shared<sphere>(point3(-1.2, 0, -1), 0.25f, earth));
    world.add(make_shared<sphere>(point3(-0.9, 0.2f, -1), 0.1f, glass));
    world.add(make_shared<sphere>(point3(-0.65, 0, -1), 0.14f, mars));
    world.add(make_shared<sphere>(point3(0.3, 0, -1), 0.65f, jupyter));
    world.add(make_shared<sphere>(point3(1.7, 0, -1), 0.55f, saturn));
    world.add(make_shared<sphere>(point3(2.8, 0, -1), 0.35f, uranus));

    // Ray trace
    for (int j = 0; j < height; j++) {
        for (int i = 0; i < width; i++) {
            color c(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; s++) // antialias
            {
                float u = float(i + random_float()) / (width - 1);
                float v = float(height - j - 1 - random_float()) / (height - 1);

                ray r = cam.get_ray(u, v);
                c += ray_color(r, world, max_depth);
            }
            c = normalize_color(c, samples_per_pixel);
            image.set_vec3(j, i, c);
        }
    }

    image.save("demo.png");
}
