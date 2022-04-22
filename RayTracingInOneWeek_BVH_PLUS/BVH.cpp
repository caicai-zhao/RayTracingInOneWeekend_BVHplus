#include <algorithm>
#include <cassert>
#include "BVH.h"

BVHAccel::BVHAccel(std::vector<shared_ptr<hittable>>_p, int _maxPrimsInNode, SplitMethod _splitMethod)
	:maxPrimsInNode(std::min(255, _maxPrimsInNode)), splitMethod(_splitMethod), primitives(std::move(_p)) {
	time_t start, stop;
	time(&start);
	if (primitives.empty())
		return;
	root = recursiveBuild(primitives);		//核心代码，输入参数为 存储指向obj的指针的vector， 构建一个bvh tree，返回根节点指针。
	time(&stop);

	double diff = difftime(stop, start);
	int hrs = (int)diff / 3600;
	int mins = ((int)diff / 60) - (hrs * 60);
	int secs = (int)diff - (hrs * 3600) - (mins * 60);

	std::cerr<<"\rBVH Generation complete: \nTime Taken:"<< hrs<<"hrs, "<< mins<<"mins, "<<secs<<"secs\n\n";
}

BVHBuildNode* BVHAccel::recursiveBuild(std::vector<shared_ptr<hittable>> _objects) {
	BVHBuildNode* node = new BVHBuildNode();
	std::cerr << "_objects.size() = "<<_objects.size() << std::endl;
	if (_objects.size() == 1) {
		node->bounds = _objects[0]->getBounds();
		node->object = _objects[0];
		node->left = nullptr;
		node->right = nullptr;
		return node;
	}
	else if (_objects.size() == 2) {
		node->left = recursiveBuild(std::vector<shared_ptr<hittable>>{ _objects[0] });
		node->right = recursiveBuild(std::vector<shared_ptr<hittable>>{ _objects[1] });
		node->bounds = Union(node->left->bounds, node->right->bounds); //每一个父节点的bound 是子节点 bound的并集。
		return node;
	}
	else {
		Bounds3 centroidBounds;
		for (int i = 0; i < _objects.size(); ++i)
			centroidBounds = Union(centroidBounds, _objects[i]->getBounds().Centroid());
		int dim = centroidBounds.maxExtent();  //此处获知中心盒子沿着哪个轴最宽。
		
		switch (dim) { //根据dim对 obj进行排序
		case 0 :
			std::sort(_objects.begin(), _objects.end(), [](auto f1, auto f2) {
				return f1->getBounds().Centroid().x() <
					f2->getBounds().Centroid().x();
				});
			break;
		case 1:
			std::sort(_objects.begin(), _objects.end(), [](auto f1, auto f2) {
				return f1->getBounds().Centroid().y() <
					f2->getBounds().Centroid().y();
				});
			break;
		case 2:
			std::sort(_objects.begin(), _objects.end(), [](auto f1, auto f2) {
				return f1->getBounds().Centroid().z() <
					f2->getBounds().Centroid().z();
				});
			break;
		}

		auto beginning = _objects.begin();
		auto middling = _objects.begin() + (_objects.size() / 2);
		auto ending = _objects.end();

		auto leftshapes = std::vector<shared_ptr<hittable>>(beginning, middling);
		auto rightshapes = std::vector<shared_ptr<hittable>>(middling, ending);
		assert(_objects.size() == (leftshapes.size() + rightshapes.size()));//二分最好加一个assert


		node->left = recursiveBuild(leftshapes);
		node->right = recursiveBuild(rightshapes);  // 递归建树
		node->bounds = Union(node->left->bounds, node->right->bounds);
	}
	return node;
}

hit_record BVHAccel::hit(const ray& _r)const {
	hit_record hit_point;
	if (!root)
		return hit_point;
	hit_point = BVHAccel::get_hit_point(root, _r); //输入bvh树根节点和光线，返回光线与场景交点（可能为空）
	return hit_point;
}

hit_record BVHAccel::get_hit_point(BVHBuildNode* _node, const ray& _r)const {
	hit_record hit_point;
	vec3 invdir(_r.direction_inv.x(), _r.direction_inv.y(), _r.direction_inv.z());
	std::array<int, 3> dirIsNeg;
	dirIsNeg[0] = _r.direction().x() < 0;
	dirIsNeg[1] = _r.direction().y() < 0;
	dirIsNeg[2] = _r.direction().z() < 0;

	if (!_node->bounds.IntersectP(_r, invdir, dirIsNeg))	//判断光线是否进入了bvh节点的box，进入则进一步求交。
		return hit_point;
	if (_node->left == nullptr && _node->right == nullptr)
		return _node->object->get_hit_point(_r);	//在一层一层判断光线与子节点的box相交后，最终递归至单个box，在单个box内调用基础对象（本场景为球，MeshTriangle中为基础三角形）
	hit_record hit1 = get_hit_point(_node->left, _r);
	hit_record hit2 = get_hit_point(_node->right, _r);
	return hit1.t < hit2.t ? hit1 : hit2;

}