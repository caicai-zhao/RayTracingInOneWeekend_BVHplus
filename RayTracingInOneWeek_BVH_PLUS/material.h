#ifndef MATERIAL_H
#define MATERIAL_H
#include "rtweekend.h"

struct hit_record;

class material {
public:
	virtual bool scatter(const ray& _r_in, const hit_record& _rec, color& _attenuation, ray& _scattered) const = 0;
};

class lambertian : public material {
public:
    lambertian(const color& a) : albedo(a) {}

    virtual bool scatter(
        const ray& _r_in, const hit_record& _rec, color& _attenuation, ray& _scattered
    ) const override {
        auto scatter_direction =  random_in_hemisphere(_rec.normal);

        // Catch degenerate scatter direction
        if (scatter_direction.near_zero())
            scatter_direction = _rec.normal;
 
        _scattered = ray(_rec.p + _rec.normal * EPSILON, scatter_direction);
        _attenuation = albedo;
        return true;
    }

public:
    color albedo;
};

class metal :public material {
public: 
    metal(const color& a, double f) :albedo(a),fuzz(f<1? f : 1) {}

    virtual bool scatter(const ray& _r_in, const hit_record& _rec, color& _attenuation, ray& _scattered)const override {
        vec3 reflected = reflect(unit_vector(_r_in.direction()), _rec.normal);
        _scattered = ray(_rec.p+_rec.normal * EPSILON, reflected + fuzz *random_in_unit_sphere());
        _attenuation = albedo;
        return (dot(_scattered.direction(), _rec.normal) > 0);
    }

public:
    color albedo;
    double fuzz;
};

class dielectric : public material {
public:
    dielectric(double index_of_refraction) :ir(index_of_refraction) {}
    virtual bool scatter(const ray& _r_in, const hit_record& _rec, color& _attenuation, ray& _scattered)const override {
        _attenuation = color(1.0, 1.0, 1.0);
        double refraction_ratio = _rec.front_face ? (1.0 / ir) : ir;
        vec3 unit_direction = unit_vector(_r_in.direction());

        double cos_theta = fmin(dot(-unit_direction, _rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
        bool cannot_refract = refraction_ratio * sin_theta > 1.0 ;
        vec3 direction;

        if (cannot_refract||reflectance(cos_theta,refraction_ratio)>random_double())
            direction = reflect(unit_direction, _rec.normal);
        else
            direction = refract(unit_direction, _rec.normal, refraction_ratio);
        _scattered = ray(_rec.p, direction);
        return true;
    }

public:
    double ir; // Index of Refraction


private:
    static double reflectance(double cosine, double ref_idx) {
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow(1 - cosine, 5);
    }
};

#endif // !MATERIAL_H
#pragma once
