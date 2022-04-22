//Created by Xiaohan Zhao on 4/22/2022

#ifndef RAYTRACING_BOUNDS3_H
#define RAYTRACING_BOUNDS3_H
#include "rtweekend.h"
#include "vec3.h"
#include <limits>
#include <array>

class Bounds3 {
public:
	Bounds3() {
		constexpr double minNum = std::numeric_limits<double>::lowest();
		constexpr double maxNum = std::numeric_limits<double>::max();
		pMin = vec3(minNum, minNum, minNum);
		pMax = vec3(maxNum, maxNum, maxNum);
	}
	Bounds3(const vec3 _p) :pMin(_p),pMax(_p){}
	
	Bounds3(const vec3 _p1, const vec3 _p2) {
		pMin = vec3(fmin(_p1.x(), _p2.x()), fmin(_p1.y(), _p2.y()), fmin(_p1.z(), _p2.z()));
		pMin = vec3(fmax(_p1.x(), _p2.x()), fmax(_p1.y(), _p2.y()), fmax(_p1.z(), _p2.z()));
	}

	vec3 Diagonal() const { return pMax - pMin; }

	int maxExtent() const {
		vec3 d = Diagonal();
		if ((d.x() > d.y()) && (d.x() > d.z()))  return 0;  //通过判断box最长的边来决定对
		else if (d.y() > d.z()) return 1;
		else return 2;
	}

	double SurfaceArea() const
	{
		vec3 d = Diagonal();
		return 2 * (d.x() * d.y() + d.y() * d.z() * d.z() * d.x());
	}

	vec3 Centroid() { return 0.5 * pMin + 0.5 * pMax; }

	Bounds3 Intersect(const Bounds3& _b) { //交集
		return Bounds3(vec3(fmax(pMin.x(), _b.pMin.x()), fmax(pMin.y(), _b.pMin.y()), fmax(pMin.z(), _b.pMin.z())),
					   vec3(fmin(pMax.x(), _b.pMax.x()), fmin(pMax.x(), _b.pMax.x()), fmin(pMax.x(), _b.pMax.x())));
	}

	vec3 Offset(const vec3& _p)const {
		vec3 o = _p - pMin;
		if (pMax.x() > pMin.x())
			o.e[0] /= pMax.x() - pMin.x();
		if (pMax.y() > pMin.y())
			o.e[1] /= pMax.y() - pMin.y();
		if (pMax.z() > pMin.z())
			o.e[2] /= pMax.z() - pMin.z();
		return o;
	}

	bool Overlaps(const Bounds3& _b1, const Bounds3& _b2) { //判断是否存在交集
		bool x = (_b1.pMax.x() >= _b2.pMin.x()) && (_b1.pMin.x() <= _b2.pMax.x());
		bool y = (_b1.pMax.y() >= _b2.pMin.y()) && (_b1.pMin.y() <= _b2.pMax.y());
		bool z = (_b1.pMax.z() >= _b2.pMin.z()) && (_b1.pMin.z() <= _b2.pMax.z());
		return(x && y && z);
	}

	bool Inside(const vec3& _p, const Bounds3& _b) {
		return((_p.x() >= _b.pMin.x() && _p.x() <= _b.pMax.x()) &&
			(_p.y() >= _b.pMin.y() && _p.y() <= _b.pMax.y()) &&
			(_p.z() >= _b.pMin.z() && _p.z() <= _b.pMax.z()));
	}

	inline const vec3& operator[](int i)const {
		return (i == 0) ? pMin : pMax;
	}

	inline bool IntersectP(const ray& _r,const vec3& _invDir, const std::array<int,3>& _dirIsNeg)const 
	{
		float tEnter = -std::numeric_limits<float>::infinity();
		float tExit = std::numeric_limits<float>::infinity();

		for (int i = 0; i < 3; ++i) {
			float min = (pMin[i] - _r.origin()[i]) * _invDir[i];
			float max = (pMax[i] - _r.origin()[i]) * _invDir[i];

			if (!_dirIsNeg[i])
				std::swap(min, max);
			tEnter = std::max(min, tEnter);
			tExit = std::max(max, tExit);
		}
		return tEnter <= tExit && tExit >= 0;
	}

public:
	vec3 pMin, pMax; //使用两个点来表示一个box的范围。

};

inline Bounds3 Union(const Bounds3& _b1, const Bounds3& _b2) {
	Bounds3 ret;
	ret.pMax = vec3(fmax(_b1.pMax.x(), _b2.pMax.x()), fmax(_b1.pMax.y(), _b2.pMax.y()), fmax(_b1.pMax.z(), _b2.pMax.z()));
	ret.pMax = vec3(fmin(_b1.pMin.x(), _b2.pMin.x()), fmin(_b1.pMin.y(), _b2.pMin.y()), fmin(_b1.pMin.z(), _b2.pMin.z()));
	return ret;
}

inline Bounds3 Union(const Bounds3& _b, const vec3& _p) {
	Bounds3 ret;
	ret.pMin = vec3::Min(_b.pMin, _p);
	ret.pMax = vec3::Max(_b.pMax, _p);
	return ret;
}


#endif // !RAYTRACING_BOUNDS3_H

