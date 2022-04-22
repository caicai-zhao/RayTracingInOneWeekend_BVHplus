//
// Created by Xiaohan_Zhao on 4/22/2022.
//

#include "Scene.h"
#include "material.h"

void Scene::buildBVH() {
    std::cerr<<(" - Generating BVH...\n\n");
    this->bvh = new BVHAccel(objects, 1, BVHAccel::SplitMethod::NAIVE);
}

hit_record Scene::intersect(const ray& ray) const
{
    return this->bvh->hit(ray); 
}


// Implementation of the Whitted-syle light transport algorithm (E [S*] (D|G) L)
//
// This function is the function that compute the color at the intersection point
// of a ray defined by a position and a direction. Note that thus function is recursive (it calls itself).
//
// If the material of the intersected object is either reflective or reflective and refractive,
// then we compute the reflection/refracton direction and cast two new rays into the scene
// by calling the castRay() function recursively. When the surface is transparent, we mix
// the reflection and refraction color using the result of the fresnel equations (it computes
// the amount of reflection and refractin depending on the surface normal, incident view direction
// and surface refractive index).
//
// If the surface is duffuse/glossy we use the Phong illumation model to compute the color
// at the intersection point.
Vector3f Scene::castRay(const ray& _r, int depth) const
{
    if (depth < 0) {
        return Vector3f(0.0, 0.0, 0.0);
    }
    hit_record intersection = Scene::intersect(_r);  //光线与场景求交的核心代码，输入光线，返回hit_record.
    intersection.set_face_normal(_r,intersection.normal); //此行来判断是否在物体内部，如在内部，校正法线，如删除，fraction将无法正常渲染。
    shared_ptr<hittable> hitObject = intersection.obj;
    Vector2f uv;
    uint32_t index = 0;
    if (intersection.happened) {

        Vector3f hitPoint = intersection.p;
        Vector3f N = intersection.normal; // normal
       
        ray scattered;      //反射方向
        color attenuation;  //obj本身颜色。
        if (intersection.mat_ptr->scatter(_r, intersection, attenuation, scattered))
            return attenuation * castRay(scattered, depth - 1);
        return color(0, 0, 0);
    }
    else
    {
        //背景颜色。
        vec3 unit_direction = unit_vector(_r.direction());
        auto t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
    }
}