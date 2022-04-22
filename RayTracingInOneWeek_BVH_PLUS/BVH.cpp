#include <algorithm>
#include <cassert>
#include "BVH.h"

BVHAccel::BVHAccel(std::vector<shared_ptr<hittable>>_p, int _maxPrimsInNode, SplitMethod _splitMethod)
	:maxPrimsInNode(std::min(255, _maxPrimsInNode)), splitMethod(_splitMethod), primitives(std::move(_p)) {
	time_t start, stop;
	time(&start);
	if (primitives.empty())
		return;
	root = recursiveBuild(primitives);		//���Ĵ��룬�������Ϊ �洢ָ��obj��ָ���vector�� ����һ��bvh tree�����ظ��ڵ�ָ�롣
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
		node->bounds = Union(node->left->bounds, node->right->bounds); //ÿһ�����ڵ��bound ���ӽڵ� bound�Ĳ�����
		return node;
	}
	else {
		Bounds3 centroidBounds;
		for (int i = 0; i < _objects.size(); ++i)
			centroidBounds = Union(centroidBounds, _objects[i]->getBounds().Centroid());
		int dim = centroidBounds.maxExtent();  //�˴���֪���ĺ��������ĸ������
		
		switch (dim) { //����dim�� obj��������
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
		assert(_objects.size() == (leftshapes.size() + rightshapes.size()));//������ü�һ��assert


		node->left = recursiveBuild(leftshapes);
		node->right = recursiveBuild(rightshapes);  // �ݹ齨��
		node->bounds = Union(node->left->bounds, node->right->bounds);
	}
	return node;
}

hit_record BVHAccel::hit(const ray& _r)const {
	hit_record hit_point;
	if (!root)
		return hit_point;
	hit_point = BVHAccel::get_hit_point(root, _r); //����bvh�����ڵ�͹��ߣ����ع����볡�����㣨����Ϊ�գ�
	return hit_point;
}

hit_record BVHAccel::get_hit_point(BVHBuildNode* _node, const ray& _r)const {
	hit_record hit_point;
	vec3 invdir(_r.direction_inv.x(), _r.direction_inv.y(), _r.direction_inv.z());
	std::array<int, 3> dirIsNeg;
	dirIsNeg[0] = _r.direction().x() < 0;
	dirIsNeg[1] = _r.direction().y() < 0;
	dirIsNeg[2] = _r.direction().z() < 0;

	if (!_node->bounds.IntersectP(_r, invdir, dirIsNeg))	//�жϹ����Ƿ������bvh�ڵ��box���������һ���󽻡�
		return hit_point;
	if (_node->left == nullptr && _node->right == nullptr)
		return _node->object->get_hit_point(_r);	//��һ��һ���жϹ������ӽڵ��box�ཻ�����յݹ�������box���ڵ���box�ڵ��û������󣨱�����Ϊ��MeshTriangle��Ϊ���������Σ�
	hit_record hit1 = get_hit_point(_node->left, _r);
	hit_record hit2 = get_hit_point(_node->right, _r);
	return hit1.t < hit2.t ? hit1 : hit2;

}