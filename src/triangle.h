#ifndef TRIANGLE_H_
#define TRIANGLE_H_


#include "hittable.h"
#include "AGLM.h"

class triangle : public hittable {
public:
    triangle() : a(0), b(0), c(0), mat_ptr(0) {}

    triangle(const glm::point3 &v0, const glm::point3 &v1, const glm::point3 &v2,
             std::shared_ptr<material> m) : a(v0), b(v1), c(v2), mat_ptr(m) {};

    bool hit(const ray &r, hit_record &rec) const override {
        // adapted from class

        auto v0 = a, v1 = b, v2 = c;
        auto e1 = v1 - v0;
        auto e2 = v2 - v0;
        auto p = cross(r.direction(), e2);
        auto _a = dot(e1, p);

        float eps = 0.0001f;
        if (fabs(_a) < eps) {
            return false;
        }

        auto f = 1 / _a;
        auto s = r.origin() - v0;
        auto u = f * (dot(s, p));
        if (u < 0.0f || u > 1.0f) {
            return false;
        }

        auto q = cross(s, e1);
        auto v = f * dot(r.direction(), q);
        if (v < 0.0f || u + v > 1.0f) {
            return false;
        }

        auto t = f * dot(e2, q);
        rec.t = t;
        rec.p = r.at(rec.t);
        rec.mat_ptr = mat_ptr;
        glm::vec3 outward_normal = glm::vec3(0); // TODO
        rec.set_face_normal(r, outward_normal);
        return true;
    }

public:
    glm::point3 a;
    glm::point3 b;
    glm::point3 c;
    std::shared_ptr<material> mat_ptr;
};

#endif
