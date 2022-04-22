#pragma once
#ifndef VEC3_H
#define VEC3_H
#include <cmath>
#include <iostream>

using std::sqrt;
class vec3 {
public:
	vec3() :e{ 0,0,0 } {}
	vec3(double e0, double e1, double e2) :e{ e0,e1,e2 } {}

	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }

	vec3 operator -() const { return vec3(-e[0], -e[1], -e[2]); }
	double operator[](int i) const { return e[i]; }
	double& operator[](int i){ return e[i]; }

	vec3& operator += (const vec3& v) {
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}

	vec3& operator *= (const double t) {
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}

	vec3& operator/=(const double t) {
		return *this *= 1 / t;
	}

	bool near_zero() const {
		// Return true if the vector is close to zero in all dimensions.
		const auto s = 1e-8;
		return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
	}


	double length_squared() const {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	double length()const {
		return sqrt(length_squared());
	}

	void write_color(std::ostream& out) {
		out << static_cast<int>(255.999 * e[0]) << ' '
			<< static_cast<int>(255.999 * e[1]) << ' '
			<< static_cast<int>(255.999 * e[2]) << '\n';
	}

	inline static vec3 random() {
		return vec3(random_double(), random_double(), random_double());
	}
	inline static vec3 random(double min, double max) {
		return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}

	static vec3 Min(const vec3& _p1, const vec3& _p2) {
		return vec3(std::min(_p1.x(), _p2.x()), std::min(_p1.y(), _p2.y()),
			std::min(_p1.z(), _p2.z()));
	}

	static vec3 Max(const vec3& _p1, const vec3& _p2) {
		return vec3(std::max(_p1.x(), _p2.x()), std::max(_p1.y(), _p2.y()),
			std::max(_p1.z(), _p2.z()));
	}


public:
	double e[3];
};

using point3 = vec3;
using color = vec3;


inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) {return vec3(u.e[0]+ v.e[0], u.e[1]+v.e[1], u.e[2] + v.e[2]);}
inline vec3 operator-(const vec3& u, const vec3& v) {return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);}
inline vec3 operator*(const vec3& u, const vec3& v) {return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);}
inline vec3 operator*(double t, const vec3& v) {return vec3(t * v[0], t * v[1], t * v[2]);}
inline vec3 operator*(const vec3& v, double t) {return t * v;}
inline vec3 operator/(vec3 v, double t) { return (1 / t) * v; }

inline double dot(const vec3& u, const vec3& v) { 
	return u.e[0] * v.e[0] + 
		   u.e[1] * v.e[1] + 
		   u.e[2] * v.e[2]; 
}

inline vec3 cross(const vec3& u, const vec3& v) {
	return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
				u.e[2] * v.e[0] - u.e[0] * v.e[2],
				u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(vec3 v) { return v / v.length(); }

inline vec3 random_in_unit_disk() {
	while (true) {
		auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
		if (p.length_squared() >= 1) continue;
		return p;
	}
}

inline vec3 random_in_unit_sphere() {
	while (true) {
		auto p = vec3::random(-1, 1);
		if (p.length_squared() >= 1) continue;
		return p;
	}
}

inline vec3 random_unit_vector() {
	return unit_vector(random_in_unit_sphere());
}

inline vec3 random_in_hemisphere(const vec3& normal) {
	vec3 in_unit_sphere = random_in_unit_sphere();
	if (dot(in_unit_sphere, normal) > 0)
		return in_unit_sphere;
	else
		return -in_unit_sphere;
}

inline vec3 reflect(const vec3& v, const vec3& n) {
	return v - 2 * dot(v, n) * n;
}


inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
	auto cos_theta = fmin(dot(-uv, n), 1.0);
	vec3 R_out_perp = etai_over_etat * (uv + n * cos_theta);
	vec3 R_out_parallel = -sqrt(1 - R_out_perp.length_squared()) * n;
	return R_out_parallel + R_out_perp;
}

inline vec3 lerp(const vec3 & a, const vec3& b, const float& t)
{
	return a * (1 - t) + b * t;
}

#endif