#include "rtweekend.h"
//#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "color.h"
#include "material.h"
#include "Scene.h"
#include <iostream>
#include <fstream>


Scene random_scene() {
    Scene world(1200,800);

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.Add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.Add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.Add(make_shared<sphere>(center, 0.2, sphere_material));
                }
                else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.Add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.Add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.Add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.Add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}


int main() {
    time_t start, stop;
    time(&start);
    // Image
    
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 10;
     
    Scene world(image_width,image_height);
 
    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left = make_shared<dielectric>(1.5);
    auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

    world.Add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.Add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
    world.Add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.45, material_left));
    world.Add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

    
    world.buildBVH();   //����BVH�������ٽṹ��

    point3 lookfrom(0,0,0);
    point3 lookat(0, 0, -1);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = (lookfrom - lookat).length();
    auto aperture = 2.0;
    
    
    /*  ����������Ⱦͼ��ʹ��ԭ�ļ���ȾЧ�����������ڱ�����ȫ�����򣬽���400�������ʹ��BVH
    *   ��û�ж�ϵͳ�������������ƣ������Ƕ�box�󽻵Ȳ��������˳���������ٶȡ�
    *   �ܶ���֮�����Բ�����ɹ����������Ҽ����˶�BVH�͹���׷�ٻ�������⡣
    *   ���ʹ��MeshTriangle��������Ϊ������ģ�ͣ����к�������������Ҫ�������ʱ��BVH���ܶ�
    *   ϵͳ�������ܴ���������ߣ��Ͼ�����ÿ�����ߵĴ����Ӷ���  O(n)*�󽻸��Ӷ� ��������  O��log��n����*�󽻸��Ӷ�

    //Image
    const auto aspect_ratio = 3.0 / 2.0;
    const int image_width = 1200;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 10;
    const int max_depth = 10;

 
     //World
    auto world = random_scene();
    world.buildBVH();
     
    //Camera
    point3 lookfrom(13, 2, 3);
    point3 lookat(0, 0, 0);
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10;
    auto aperture = 0.1;
    */

    //�˴����������������ʹ��һ���࣬���������ĸ��ֲ������л��ܣ�����Ⱦ���ͼƬ��
    camera cam(lookfrom, lookat, vup, 90, aspect_ratio, aperture, dist_to_focus);
    // Render


    //�����ǽ�������ݱ��浽ppm�ļ������˵����P3��ָÿ��������3����ֵ�������洢ɫ�ʣ�Ҳ����RGB��
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    int m = 0;
    for (int j = world.height - 1; j >=0; --j) {
        //����cout�������������ppm �ļ���������write_color���������˴�ʹ��cerr�����ָʾ�ı���
        std::cerr << "\rScanlines remaining: " <<  j << ' ' << std::flush; 
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0,0,0);

            //��ÿ���㸽������������ɵ㣬�ֱ�����������trace�����ɫ�����ս���ƽ�������������Ⱦ����
            //ͬʱҲ�� anti-alising ��һ���İ�����
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i+random_double()) / (image_width - 1);
                auto v = (j+random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);                       //������������ߣ������������Ϊ��Դ���ӽǣ����ǹ�Դ�Ĺ�׷��
                pixel_color += world.castRay(r,max_depth);       //���Ĵ��룬����һ�������ڳ�����trace�����ɫ��       
            }
            write_color(std::cout, pixel_color,samples_per_pixel);
        }
    }
   
    std::cerr << "\nDone.\n";
    time(&stop);
    double diff = difftime(stop, start);
    int hrs = (int)diff / 3600;
    int mins = ((int)diff / 60) - (hrs * 60);
    int secs = (int)diff - (hrs * 3600) - (mins * 60);

    std::cerr << "\rBVH complete: \n Total Time Taken:" << hrs << "hrs, " << mins << "mins, " << secs << "secs\n\n";
}
