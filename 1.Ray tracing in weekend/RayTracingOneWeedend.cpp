// RayTracingOneWeedend.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include <iostream>
//
//double hit_shpere(const point3& center, double radius, const ray& r)
//{
//    vec3 oc = r.origin() - center;
////    auto a = dot(r.direction(), r.direction());
//    auto a = r.direction().length_squared();
//    auto half_b = dot(oc, r.direction());
////    auto b = 2.0 * dot(oc, r.direction());
////    auto c = dot(oc, oc) - radius * radius;
//    auto c = oc.length_squared() - radius * radius;
//
//    auto discriminant = half_b * half_b - a*c;
//    if (discriminant < 0)
//    {
//        return -1.0;
//    }
//    else
//    {
//        return (-half_b - sqrt(discriminant)) / a;
//    }
//}

//color ray_color(const ray& r)
//{
//    auto t = hit_shpere(point3(0, 0, -1), 0.5, r);
//    if (t > 0.0)
//    {
//        vec3 N = unit_vector(r.at(t) - vec3(0, 0, -1));
//        return 0.5 * color(N.x() + 1, N.y() + 1, N.z() + 1);
//    }
//
//
////    if (hit_shpere(point3(0, 0, -1), 0.5, r))
////    {
////        return color(1, 0, 0);
////    }
//    vec3 unit_direction = unit_vector(r.direction());
//
//    t = 0.5 * (unit_direction.y() + 1.0);
//
//    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
//}

color ray_color(const ray& r, const hittable& world, int depth)
{
    hit_record rec;
    if (depth <= 0)
    {
        return vec3(0, 0, 0);
    }

    //现在这个tmin , tmax 仿佛类似于世界的边框，AABB的感觉
    if (world.hit(r, 0.001, infinity, rec))
    {
       // return 0.5 * (rec.normal + color(1, 1, 1));
        //1.在单位球内的随机一个点
        //point3 target = rec.p + rec.normal + random_in_unint_shpere();

        //2.在球面上的随机一个点
        //point3 target = rec.p + rec.normal + random_unit_vector();

        //3. 在1中改为半球的随机点
        ray scattered;
        color attenuation;
        //point3 target = rec.p + rec.normal + random_in_hemisphere(rec.normal);

        //通过递归来实现光线的不断弹射,直到碰到背景
        //scatter中 , 如果散射的方向和Normal点乘为负则终止
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            //return 0.5 * ray_color(ray(rec.p, target - rec.p), world,depth - 1);
            return attenuation * ray_color(scattered, world, depth - 1);
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}


int main()
{
    //Image
    const auto aspect_ratio = 16.0 / 9.0; //width:height
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    //World
    hittable_list world;
    //world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    //world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.8));
    //散射和金属材质
    auto material_center = make_shared<lambertian>(color(0.7, 0.3, 0.3));
//    auto material_left = make_shared<metal>(color(0.8, 0.8, 0.8));

    //玻璃材质
//    auto material_center = make_shared<dielectric>(1.5);
    auto material_left = make_shared<dielectric>(1.5);
    auto material_right = make_shared<metal>(color(0.8, 0.6, 0.2));

    world.add(make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3(0.0, 0.0, -1.0), 0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));



    //Camera
    /*
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal / 2 - vertical / 2 - vec3(0, 0, focal_length);
    */
    camera cam;
    


    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";


    for (int j = image_height - 1; j >= 0; --j)
    {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i)
        {
            /*auto r = double(i) / (image_width - 1);
            auto g = double(j) / (image_height - 1);
            auto b = 0.25;

            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);

            std::cout << ir << ' ' << ig << ' ' << ib << '\n';*/

            //

            //color pixel_color(double(i) / (image_width - 1), double(j) / (image_height - 1), 0.25);
            //write_color(std::cout, pixel_color);
            
           
            color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s)
            {
                auto u = double(i + random_double()) / (image_width - 1);
                auto v = double(j + random_double()) / (image_height - 1);

                ray r = cam.get_ray(u,v);
                pixel_color += ray_color(r, world,max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);


            //ray r(origin, lower_left_corner + u * horizontal + v * vertical - origin);
            // color pixel_color = ray_color(r);
            //color pixel_color = ray_color(r, world);
            //write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nDone.\n";
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
