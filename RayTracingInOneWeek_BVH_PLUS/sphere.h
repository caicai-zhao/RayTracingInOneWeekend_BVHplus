#ifndef SPHERE_H
#define SPHERE_H
#include "hittable.h"
#include "vec3.h"
class sphere : public hittable {
public:
	sphere() {}
	sphere(point3 _cen, double _r,shared_ptr<material> _m) :center(_cen), radius(_r), mat_ptr(_m) {}
	
	bool hit(const ray& _r) {
		// analytic solution
		Vector3f L = _r.origin() - center;
		float a = dot(_r.direction(), _r.direction());
		float half_b =  dot(_r.direction(), L);
		float c = dot(L, L) - radius*radius;

		auto discriminant = half_b * half_b - a * c;
		if (discriminant < 0) return false;
		auto sqrtd = sqrt(discriminant);

		auto root = (-half_b - sqrtd) / a;
		if (root<0) {
			root = (-half_b + sqrtd) / a;
			if (root<0)
				return false;
		}
		return true;
	}

	bool hit(const ray& _r, float& _tnear, uint32_t& _index)const {
		// analytic solution
		Vector3f L = _r.origin() - center;
		float a = dot(_r.direction(), _r.direction());
		float half_b = dot(_r.direction(), L);
		float c = dot(L, L) - radius * radius;

		auto discriminant = half_b * half_b - a * c;
		if (discriminant < 0) return false;
		auto sqrtd = sqrt(discriminant);

		auto root = (-half_b - sqrtd) / a;
		if (root < 0) {
			root = (-half_b + sqrtd) / a;
			if (root < 0)
				return false;
		}
		_tnear = root;
		return true;
	}

	hit_record get_hit_point(ray _r) {
		hit_record rec;
		rec.happened = false;
		Vector3f L = _r.origin() - center;
		float a = dot(_r.direction(), _r.direction());
		float half_b = dot(_r.direction(), L);
		float c = dot(L, L) - radius * radius;

		auto discriminant = half_b * half_b - a * c;
		if (discriminant < 0) return rec;
		auto sqrtd = sqrt(discriminant);

		auto root = (-half_b - sqrtd) / a;
		if (root < 0.001) {
			root = (-half_b + sqrtd) / a;
			if (root < 0.001)			//此处设置裕量可以有效减少途中自遮挡引起的黑点。
				return rec;
		}
		rec.happened = true;

		rec.p = vec3(_r.orig + _r.dir * root);
		rec.normal = unit_vector(vec3(rec.p - center));
		rec.mat_ptr = this->mat_ptr;
		*rec.obj = *this;
		rec.t = root;
		return rec;
	}

	void getSurfaceProperties(const Vector3f& P, const Vector3f& I, const uint32_t& index, const Vector2f& uv, Vector3f& N, Vector2f& st) const
	{
		N = unit_vector(P - center);
	}

	Bounds3 getBounds() {
		return Bounds3(Vector3f(center.x() - radius, center.y() - radius, center.z() - radius),
			Vector3f(center.x() + radius, center.y() + radius, center.z() + radius));
	}

public:
	point3 center;
	double radius;
	shared_ptr<material> mat_ptr;
};

#endif // !SPHERE_H

