//Creat by Xiaohan Zhao on 4/22/2022

#ifndef RAYTRACING_BVH_H
#define RAYTRACING_BVH_H
#include <atomic>
#include <vector>
#include <memory>
#include <ctime>
#include "hittable.h"
#include "rtweekend.h"
#include "Bounds3.h"
#include "vec3.h"

struct BVHBuildNode;
struct BVHPrimitiveInfo;

//inline int leafNodes, totalLeafNodes, totalPrimitives, interiorNodes;
class BVHAccel {
public:
	enum class SplitMethod {NAIVE,SAH};
	BVHAccel(std::vector<shared_ptr<hittable>> _p, int _maxPrimsInNode = 1, SplitMethod _splitMethod = SplitMethod::NAIVE);
	Bounds3 WorldBound()const;
	~BVHAccel();

	hit_record hit(const ray& _r)const;
	hit_record get_hit_point(BVHBuildNode* _node, const ray& _r)const;

	bool IntersectP(const ray& _r)const;
	BVHBuildNode* root;
	BVHBuildNode* recursiveBuild(std::vector<shared_ptr<hittable>> _objects);

	const int maxPrimsInNode;
	const SplitMethod splitMethod;
	std::vector<shared_ptr<hittable>> primitives;
};

struct  BVHBuildNode
{
	Bounds3 bounds;
	BVHBuildNode* left;
	BVHBuildNode* right;
	shared_ptr<hittable> object;

public:
	int splitAxis = 0, firstPrimOffset = 0, nPrimitives = 0;
	BVHBuildNode() {
		bounds = Bounds3();
		left = nullptr; right = nullptr;
		object = nullptr;
	}
};

#endif // RAYTRACING_BVH_H
