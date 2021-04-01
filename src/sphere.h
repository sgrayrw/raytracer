// ray.h, from https://raytracing.github.io by Peter Shirley, 2018-2020
#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "AGLM.h"

class sphere : public hittable {
public:
    sphere() : radius(0), center(0), mat_ptr(0) {}

    sphere(const glm::point3 &cen, float r, std::shared_ptr<material> m) :
            center(cen), radius(r), mat_ptr(m) {};

    bool hit(const ray &r, hit_record &rec) const override;
    virtual bool hit_analytic(const ray &r, hit_record &rec) const; // just for reference

public:
    glm::point3 center;
    float radius;
    std::shared_ptr<material> mat_ptr;
};

bool sphere::hit_analytic(const ray &r, hit_record &rec) const {
    glm::vec3 oc = r.origin() - center;
    float a = glm::dot(r.direction(), r.direction());
    float half_b = glm::dot(oc, r.direction());
    float c = glm::length2(oc) - radius * radius;

    float discriminant = half_b * half_b - a * c;
    if (discriminant < 0) return false;
    float sqrtd = sqrt(discriminant);

    float t = (-half_b - sqrtd) / a;
    if (t < 0) t = (-half_b + sqrtd) / a;
    if (t < 0) return false;

    // save relevant data in hit record
    rec.t = t; // save the time when we hit the object
    rec.p = r.at(t); // ray.origin + t * ray.direction
    rec.mat_ptr = mat_ptr;

    // save normal
    glm::vec3 outward_normal = normalize(rec.p - center); // compute unit length normal
    rec.set_face_normal(r, outward_normal);

    return true;
}

bool sphere::hit(const ray &r, hit_record &rec) const {
    // geometric method, adapted from class
    glm::point3 l = center - r.origin();
    float s = glm::dot(l, normalize(r.direction()));
    float lSqr = glm::dot(l, l);
    float rSqr = radius * radius;
    if (s < 0 && lSqr > rSqr) {
        return false;
    }

    float mSqr = lSqr - s * s;
    if (mSqr > rSqr) {
        return false;
    }

    float q = sqrt(rSqr - mSqr);

    rec.t = (lSqr > rSqr) ? s - q : s + q;
    rec.t /= glm::length(r.direction());
    rec.p = r.at(rec.t);
    rec.mat_ptr = mat_ptr;
    glm::vec3 outward_normal = normalize(rec.p - center); // compute unit length normal
    rec.set_face_normal(r, outward_normal);

    return true;
}

#endif

