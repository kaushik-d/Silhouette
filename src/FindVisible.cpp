#include <iostream>
#include "FindVisible.h"
#include "BasisTransformation.h"

std::vector<STL::Triangle> FindVisible::get_visible_list(const std::vector<STL::Triangle> &triangles, const Eigen::Vector3d &shadow_plane_normal)
{
    std::vector<STL::Triangle> visible_trianlges;

    //Eigen::Vector3d light_direction = -shadow_plane_normal;

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

mpolygon_t FindVisible::get_shadow(const std::vector<STL::Triangle> &triangles, const Eigen::Vector3d &shadow_plane_normal, const Eigen::Vector3d &shadow_plane_origin)
{

    BasisTransformation transformation(shadow_plane_normal, shadow_plane_origin);

    auto triangle = triangles[0];
    // triangle.project(shadow_plane_normal, shadow_plane_origin);
    polygon_t shadow1 = triangle.toPolygon2D(transformation);

    mpolygon_t shadow;
    shadow.push_back(shadow1);

    for (int i = 1; i < triangles.size(); i++)
    {
        auto triangle = triangles[i];
        // triangle.project(shadow_plane_normal, shadow_plane_origin);

        mpolygon_t union_poly;
        polygon_t tri_poly = triangle.toPolygon2D(transformation);


        if (bg::area(tri_poly) < 0.00001)
        {
            continue;
        }

        boost::geometry::union_(tri_poly, shadow, union_poly);
        
        std::cout << " tri_poly area = " << bg::area(tri_poly) << std::endl;
        std::cout << " shadow area = " << bg::area(shadow) << std::endl;
        std::cout << " union_poly area = " << bg::area(union_poly) << std::endl;        

        shadow = union_poly;

        bg::correct(shadow);
    }

    return shadow;
}

void FindVisible::project_tringles(std::vector<STL::Triangle> &triangles, const Eigen::Vector3d &shadow_plane_normal, const Eigen::Vector3d &shadow_plane_origin)
{
    //auto triangle = triangles[0];
    //triangle.project(shadow_plane_normal, shadow_plane_origin);

    for (auto &triangle : triangles)
    {
        triangle.project(shadow_plane_normal, shadow_plane_origin);
    }
}
