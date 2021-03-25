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
        using namespace glm;

        // v1
        //vec3 unitn = normalize(rec.normal);
        //// TODO lightPos
        //vec3 lightPos = vec3(5, 5, 0);
        //vec3 lightDir = normalize(lightPos - rec.p);
        //color diffuse = max(vec3(0), dot(unitn, lightDir)) * albedo;
        //attenuation = diffuse;
        //return false;

        // v2
        vec3 scatter_dir = rec.normal + random_unit_vector();
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

    virtual bool scatter(const ray &r_in, const hit_record &hit,
                         glm::color &attenuation, ray &scattered) const override {
        // todo
        attenuation = glm::color(0);
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

    virtual bool scatter(const ray &r_in, const hit_record &rec,
                         glm::color &attenuation, ray &scattered) const override {
        // todo
        attenuation = albedo;
        return false;
    }

public:
    glm::color albedo;
    float fuzz;
};

class dielectric : public material {
public:
    dielectric(float index_of_refraction) : ir(index_of_refraction) {}

    virtual bool scatter(const ray &r_in, const hit_record &rec,
                         glm::color &attenuation, ray &scattered) const override {
        // todo
        attenuation = glm::color(0);
        return false;
    }

public:
    float ir; // Index of Refraction
};


#endif

