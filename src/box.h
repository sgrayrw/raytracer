#ifndef BOX_H_
#define BOX_H_

#include "hittable.h"
#include "AGLM.h"

class box : public hittable {
public:
    box() : c(0), ax(0), ay(0), az(0), hx(0), hy(0), hz(0), mat_ptr(0) {}

    box(const glm::point3 &center,
        const glm::vec3 &xdir, const glm::vec3 &ydir, const glm::vec3 &zdir,
        const glm::vec3 &halfx, const glm::vec3 &halfy, const glm::vec3 &halfz,
        std::shared_ptr<material> m) : c(center), ax(xdir), ay(ydir), az(zdir),
                                       hx(halfx), hy(halfy), hz(halfz), mat_ptr(m) {};

    virtual bool hit(const ray &r, hit_record &rec) const override {
        // todo
        return false;
    }

public:
    glm::vec3 c;
    glm::vec3 ax;
    glm::vec3 ay;
    glm::vec3 az;
    glm::vec3 hx;
    glm::vec3 hy;
    glm::vec3 hz;
    std::shared_ptr<material> mat_ptr;
};

#endif
