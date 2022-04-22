#pragma once
#ifndef  HITTABLE_H
#define HITTABLE_H
#include "rtweekend.h"
#include "Bounds3.h"

class material;
class hittable;

// 6.3 对可命中对象的抽象
struct hit_record
{
	point3 p;
	vec3 normal;
	shared_ptr<material> mat_ptr;
	double t;
	shared_ptr<hittable> obj;
	bool happened;

	hit_record() {
		happened = false;
		p = vec3();
		normal = vec3();
		t = std::numeric_limits<double>::max();
		obj = nullptr;
		mat_ptr = nullptr;
	}

	
	//表侧法线还是内测法线
	bool front_face;
	inline void set_face_normal(const ray& _r, const vec3& _outward_normal) {
		front_face = dot(_r.direction(), _outward_normal) < 0;
		normal = front_face ? _outward_normal : -_outward_normal;
	}
};

class hittable { //objec基类
public:
	hittable() {}
	virtual ~hittable(){}
	
	virtual bool hit(const ray& _r) = 0;
	virtual bool hit(const ray& _r, float& _tnear, uint32_t& _index)const = 0;
	virtual hit_record get_hit_point(ray _r) = 0;	//此接口调用较多，返回光线与派生类的交点


	virtual void getSurfaceProperties(const vec3&, const vec3&, const uint32_t&, const vec2&, vec3&, vec2&)const = 0;
	//virtual vec3 evalDiffuseColor(const vec2&) const = 0;
	virtual Bounds3 getBounds() = 0; //此接口为BVH中重点，可以求得派生类的最小包围盒。
};

#endif // ! HITTABLE_H
