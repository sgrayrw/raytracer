#ifndef PLANE_H_
#define PLANE_H_

#include "hittable.h"
#include "AGLM.h"

class plane : public hittable {
public:
    plane() : a(0), n(0), mat_ptr(0) {}

    plane(const glm::point3 &p, const glm::vec3 &normal,
          std::shared_ptr<material> m) : a(p), n(normal), mat_ptr(m) {};

    bool hit(const ray &r, hit_record &rec) const override {
        // adapted from class

        // t = ((a - p0) \dot n) / (v \dot n)
        float numerator = dot(a - r.origin(), n);
        float denominator = dot(r.direction(), n);

        // check if ray parallel to plane
        if (denominator == 0) {
            if (numerator == 0) {
                rec.t = 0;
            } else {
                return false;
            }
        } else {
            rec.t = numerator / denominator;
            if (rec.t < 0) {
                return false;
            }
        }

        rec.p = r.at(rec.t);
        rec.mat_ptr = mat_ptr;
        glm::vec3 outward_normal = n;
        rec.set_face_normal(r, outward_normal);
        return true;
    }

public:
    glm::vec3 a;
    glm::vec3 n;
    std::shared_ptr<material> mat_ptr;
};

#endif
