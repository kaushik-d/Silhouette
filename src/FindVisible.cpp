#include <iostream>
#include <cmath>

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
    const double small_area_purge_tol_rel = 0.001;
    const double small_area_purge_tol_abs = 0.0001;

    BasisTransformation transformation(shadow_plane_normal, shadow_plane_origin);

    auto &triangle = triangles[0];
    // triangle.project(shadow_plane_normal, shadow_plane_origin);
    // polygon_t shadow1 = triangle.toPolygon2D(transformation);

    mpolygon_t shadow;
    // shadow.push_back(shadow1);

    for (int i = 0; i < triangles.size(); i++)
    {
        auto &triangle = triangles[i];

        mpolygon_t union_poly;
        polygon_t tri_poly = triangle.toPolygon2D(transformation, z_save);

        if (bg::area(tri_poly) < small_area_purge_tol_abs)
        {
            continue;
        }

        bool check_covered = bg::covered_by(tri_poly, shadow);

        if (check_covered)
        {
            continue;
        }

        boost::geometry::union_(tri_poly, shadow, union_poly);

        //std::cout << " tri_poly area = " << bg::area(tri_poly) << std::endl;
        //std::cout << " shadow area = " << bg::area(shadow) << std::endl;
        //std::cout << " union_poly area = " << bg::area(union_poly) << std::endl;

        shadow = union_poly;

        bg::correct(shadow);
    }

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
                    //bg::append(poly_corrected.inners(), inner_ring);
                    poly_corrected.inners().push_back(inner_ring);
                }
            }
            //bg::append(shadow_correction, poly_corrected);
            shadow_correction.push_back(poly_corrected);
        }
    }

    return shadow_correction;
}

void FindVisible::project_tringles(std::vector<STL::Triangle> &triangles, const Eigen::Vector3d &shadow_plane_normal, const Eigen::Vector3d &shadow_plane_origin)
{
    // auto triangle = triangles[0];
    // triangle.project(shadow_plane_normal, shadow_plane_origin);

    for (auto &triangle : triangles)
    {
        triangle.project(shadow_plane_normal, shadow_plane_origin);
    }
}
