#ifndef TRIANGLE_H_
#define TRIANGLE_H_


#include "hittable.h"
#include "AGLM.h"

class triangle : public hittable {
public:
    triangle() : a(0), b(0), c(0), mat_ptr(0) {}

    triangle(const glm::point3 &v0, const glm::point3 &v1, const glm::point3 &v2,
             std::shared_ptr<material> m) : a(v0), b(v1), c(v2), mat_ptr(m) {};

    virtual bool hit(const ray &r, hit_record &rec) const override {
        // todo
        return false;
    }

public:
    glm::point3 a;
    glm::point3 b;
    glm::point3 c;
    std::shared_ptr<material> mat_ptr;
};

#endif
