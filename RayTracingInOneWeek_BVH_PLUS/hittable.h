#pragma once
#ifndef  HITTABLE_H
#define HITTABLE_H
#include "rtweekend.h"
#include "Bounds3.h"

class material;
class hittable;

// 6.3 �Կ����ж���ĳ���
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

	
	//��෨�߻����ڲⷨ��
	bool front_face;
	inline void set_face_normal(const ray& _r, const vec3& _outward_normal) {
		front_face = dot(_r.direction(), _outward_normal) < 0;
		normal = front_face ? _outward_normal : -_outward_normal;
	}
};

class hittable { //objec����
public:
	hittable() {}
	virtual ~hittable(){}
	
	virtual bool hit(const ray& _r) = 0;
	virtual bool hit(const ray& _r, float& _tnear, uint32_t& _index)const = 0;
	virtual hit_record get_hit_point(ray _r) = 0;	//�˽ӿڵ��ý϶࣬���ع�����������Ľ���


	virtual void getSurfaceProperties(const vec3&, const vec3&, const uint32_t&, const vec2&, vec3&, vec2&)const = 0;
	//virtual vec3 evalDiffuseColor(const vec2&) const = 0;
	virtual Bounds3 getBounds() = 0; //�˽ӿ�ΪBVH���ص㣬����������������С��Χ�С�
};

#endif // ! HITTABLE_H
