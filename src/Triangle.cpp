#include "Triangle.h"

void STL::Triangle::project(const Eigen::Vector3d &shadow_plane_normal, const Eigen::Vector3d &shadow_plane_origin)
{
    normal = shadow_plane_normal;

    for(auto& vertex : vertex_list)
    {

        Eigen::Vector3d vec = vertex - shadow_plane_origin;

        double dist = vec.dot(shadow_plane_normal);

        vertex = vertex - dist*normal;
    }
}