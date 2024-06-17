#include <iostream>
#include <cmath>
#include <thread>

#include "FindVisible.h"
#include "BasisTransformation.h"

std::vector<STL::Triangle> FindVisible::get_visible_list(const std::vector<STL::Triangle> &triangles, const Eigen::Vector3d &shadow_plane_normal)
{
    std::vector<STL::Triangle> visible_trianlges;

    // Eigen::Vector3d light_direction = -shadow_plane_normal;

    for (auto &triangle : triangles)
    {
        if (triangle.normal.dot(shadow_plane_normal) > 0.0)
        {
            // visible triangle
            visible_trianlges.push_back(triangle);
        }
    }
    return visible_trianlges;
}

mpolygon_t FindVisible::get_shadow(const std::vector<STL::Triangle> &triangles,
                                   const Eigen::Vector3d &shadow_plane_normal,
                                   const Eigen::Vector3d &shadow_plane_origin, double &z_save)
{
    const double small_area_purge_tol_rel = 0.01;
    const double small_area_purge_tol_abs = 0.001;

    constexpr unsigned int max_num_threads = 2;
    unsigned int num_threads = std::thread::hardware_concurrency();
    if (num_threads > max_num_threads)
    {
        num_threads = max_num_threads;
    }

    if (triangles.size() < 100)
    {
        num_threads = 1;
        std::cout << "Number of triangles is " << triangles.size() << ". Only one threads will be used. " << std::endl;
    }
    else
    {
        std::cout << "Number of triangles is " << triangles.size() << ". " << num_threads << " threads will be used. " << std::endl;
    }

    int elements_per_thread = triangles.size() / num_threads;
    int remaining_elements = triangles.size() % num_threads;

    mpolygon_t shadow;

    std::vector<std::thread> threads;
    std::mutex g_mutex;

    auto merge_trianlges = [&](int start, int end)
    {
        mpolygon_t shadow_local;
        BasisTransformation transformation(shadow_plane_normal, shadow_plane_origin);

        for (int i = start; i < end; i++)
        {
            auto &triangle = triangles[i];

            mpolygon_t union_poly;
            polygon_t tri_poly = triangle.toPolygon2D(transformation, z_save);

            if (bg::area(tri_poly) < small_area_purge_tol_abs)
            {
                continue;
            }

            bool check_covered = bg::covered_by(tri_poly, shadow_local);

            if (check_covered)
            {
                continue;
            }

            boost::geometry::union_(tri_poly, shadow_local, union_poly);

            shadow_local = union_poly;

            bg::correct(shadow_local);
        }

        g_mutex.lock();
        mpolygon_t union_poly;
        boost::geometry::union_(shadow_local, shadow, union_poly);
        shadow = union_poly;
        g_mutex.unlock();
    };

    for (unsigned int i = 0; i < num_threads; ++i)
    {
        int start = i * elements_per_thread;
        int end = start + elements_per_thread;
        if (i == num_threads - 1)
        {
            end += remaining_elements;
        }
        threads.emplace_back(merge_trianlges, start, end);
    }

    for (auto &thread : threads)
    {
        thread.join();
    }

    threads.clear();

    mpolygon_t shadow_correction;

    double total_area = bg::area(shadow);
    const double small_area_purge_tol = std::min(small_area_purge_tol_abs, small_area_purge_tol_rel * total_area);

    for (polygon_t &poly : shadow)
    {
        if (bg::area(poly) > small_area_purge_tol)
        {
            polygon_t poly_corrected;
            poly_corrected.outer() = poly.outer();

            for (const auto &inner_ring : poly.inners())
            {
                double area_inner = bg::area(inner_ring);
                if (std::abs(area_inner) > small_area_purge_tol)
                {
                    poly_corrected.inners().push_back(inner_ring);
                }
            }
            shadow_correction.push_back(poly_corrected);
        }
    }

    return shadow_correction;
}

void FindVisible::project_tringles(std::vector<STL::Triangle> &triangles, const Eigen::Vector3d &shadow_plane_normal, const Eigen::Vector3d &shadow_plane_origin)
{
    for (auto &triangle : triangles)
    {
        triangle.project(shadow_plane_normal, shadow_plane_origin);
    }
}
