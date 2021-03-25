#include <iostream>
#include "ppm_image.h"
#include "AGLM.h"
#include "ray.h"

using namespace glm;
using namespace agl;
using namespace std;

color ray_color(const ray &ray) {
    vec3 unit_direction = normalize(ray.direction());

    float t = 0.5f * (unit_direction.y + 1.0f);
    color c1(0.5f, 0.7f, 1.0f);
    color c2(1.0);
    color c = c1 * (1 - t) + c2 * t;
    return c;
}

void ray_trace(ppm_image &image) {
    int height = image.height();
    int width = image.width();

    float aspect = width / (float) height;
    float world_height = 2.0f;
    float world_width = world_height * aspect;
    float focal_length = 1.0f;

    vec3 camera_pos = vec3(0);
    vec3 lower_left = camera_pos
                      - 0.5f * vec3(world_width, world_height, 0)
                      - vec3(0, 0, focal_length);

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            float u = j / ((float) width - 1);
            float v = (height - i - 1) / ((float) height - 1);
            vec3 world_pos = lower_left + vec3(u * world_width, v * world_height, 0);
            color c = ray_color(ray(camera_pos, world_pos - camera_pos));
            image.set_vec3(i, j, c);
        }
    }

    image.save("gradient.png");
}
