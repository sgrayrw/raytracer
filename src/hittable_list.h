// ray.h, from https://raytracing.github.io by Peter Shirley, 2018-2020

#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>
#include <limits>

using std::shared_ptr;
using std::make_shared;

class hittable_list {
public:
    hittable_list() {}

    hittable_list(shared_ptr<hittable> object) { add(object); }

    void clear() { objects.clear(); }

    void add(shared_ptr<hittable> object) { objects.push_back(object); }

    virtual bool hit(const ray &r, float min_t, float max_t, hit_record &rec) const;

public:
    std::vector<shared_ptr<hittable>> objects;
};

bool hittable_list::hit(const ray &r, float min_t, float max_t, hit_record &rec) const {
    hit_record temp_rec;
    bool hit_anything = false;
    float closest_so_far = max_t;

    for (const auto &object : objects) {
        if (object->hit(r, temp_rec)) {
            if (temp_rec.t >= min_t && temp_rec.t <= closest_so_far) {
                hit_anything = true;
                closest_so_far = std::min(closest_so_far, temp_rec.t);
                rec = temp_rec;
            }
        }
    }

    return hit_anything;
}

#endif

