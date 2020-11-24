#pragma once
#include "rtweekend.h"
#include "hittable.h"

class sphere :
    public hittable
{
public:
    sphere() = default;
    sphere(point3 centre, double r, shared_ptr<material> m);

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

public:
    point3 centre;
    double radius;
    shared_ptr<material> mat_ptr;
};

