#pragma once
#include "hittable.h"
#include "vec3.h"

class sphere :
    public hittable
{
public:
    sphere() = default;
    sphere(point3 centre, double r);

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

public:
    point3 centre;
    double radius;
};

