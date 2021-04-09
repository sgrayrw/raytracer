#ifndef MATERIAL_H
#define MATERIAL_H

#include <cmath>
#include "AGLM.h"
#include "ray.h"
#include "hittable.h"

class material {
public:
    virtual bool scatter(const ray &r_in, const hit_record &rec,
                         glm::color &attenuation, ray &scattered) const = 0;

    virtual ~material() {}
};

class lambertian : public material {
public:
    lambertian(const glm::color &a) : albedo(a) {}

    virtual bool scatter(const ray &r_in, const hit_record &rec,
                         glm::color &attenuation, ray &scattered) const override {
        // adapted from class
        glm::vec3 scatter_dir = rec.normal + random_unit_vector();
        if (near_zero(scatter_dir)) {
            scatter_dir = rec.normal;
        }
        scattered = ray(rec.p, scatter_dir);
        attenuation = albedo;
        return true;
    }

public:
    glm::color albedo;
};

class phong : public material {
public:
    phong(const glm::vec3 & view,
          const glm::color &idiffuseColor,
          float ishininess) :
            diffuseColor(idiffuseColor),
            specColor(1, 1, 1),
            ambientColor(.01f, .01f, .01f),
            lightPos(5, 5, 0),
            viewPos(view),
            kd(0.45), ks(0.45), ka(0.1), shininess(ishininess) {}

    phong(const glm::vec3 &view) :
            diffuseColor(0, 0, 1),
            specColor(1, 1, 1),
            ambientColor(.01f, .01f, .01f),
            lightPos(5, 5, 0),
            viewPos(view),
            kd(0.45), ks(0.45), ka(0.1), shininess(10.0) {}

    phong(const glm::color &idiffuseColor,
          const glm::color &ispecColor,
          const glm::color &iambientColor,
          const glm::point3 &ilightPos,
          const glm::point3 &iviewPos,
          float ikd, float iks, float ika, float ishininess) :
            diffuseColor(idiffuseColor),
            specColor(ispecColor),
            ambientColor(iambientColor),
            lightPos(ilightPos),
            viewPos(iviewPos), kd(ikd), ks(iks), ka(ika), shininess(ishininess) {}

    bool scatter(const ray &r_in, const hit_record &hit,
                 glm::color &attenuation, ray &scattered) const override {
        // adapted from class
        auto Ia = ka * ambientColor;

        glm::vec3 unitn = normalize(hit.normal);
        glm::vec3 lightDir = normalize(lightPos - hit.p);
        auto Id = kd * fmax(0.0f, dot(lightDir, unitn)) * diffuseColor;

        glm::vec3 viewDir = normalize(viewPos - hit.p);
        glm::vec3 r = normalize(reflect(lightDir, unitn));
        auto vr = dot(viewDir, r);
        if (vr > 0) {
            return false;
        }

        auto Is = ks * specColor * pow(vr, shininess);

        auto color = Ia + Id + Is;

        attenuation = color;
        return false;
    }

public:
    glm::color diffuseColor;
    glm::color specColor;
    glm::color ambientColor;
    glm::point3 lightPos;
    glm::point3 viewPos;
    float kd;
    float ks;
    float ka;
    float shininess;
};

class metal : public material {
public:
    metal(const glm::color &a, float f) : albedo(a), fuzz(glm::clamp(f, 0.0f, 1.0f)) {}

    bool scatter(const ray &r_in, const hit_record &rec,
                 glm::color &attenuation, ray &scattered) const override {
        // adapted from raytracer book
        glm::vec3 reflected = reflect(normalize(r_in.direction()), rec.normal);
        scattered = ray(rec.p, reflected + fuzz * random_unit_sphere());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

public:
    glm::color albedo;
    float fuzz;
};

class dielectric : public material {
public:
    dielectric(float index_of_refraction) : ir(index_of_refraction) {}

    bool scatter(const ray &r_in, const hit_record &rec,
                 glm::color &attenuation, ray &scattered) const override {
        // adapted from raytracer book
        attenuation = glm::color(1.0, 1.0, 1.0);
        float refraction_ratio = rec.front_face ? (1.0f / ir) : ir;

        glm::vec3 unit_direction = normalize(r_in.direction());
        double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta*cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        glm::vec3 direction;

        if (cannot_refract) {
            direction = reflect(unit_direction, rec.normal);
        }
        else {
            direction = refract(unit_direction, rec.normal, refraction_ratio);
        }

        scattered = ray(rec.p, direction);
        return true;
    }

public:
    float ir; // Index of Refraction
};


#endif

