#include "FindVisible.h"

std::vector<STL::Triangle> get_visible_list(const std::vector<STL::Triangle>& triangles, const Eigen::Vector3d& shadow_plane_normal)
{
    std::vector<STL::Triangle> visible_trianlges;

    for (auto &triangle : triangles)
    {
        if (triangle.normal.dot(shadow_plane_normal) > 0.0)
        {
            //visible triangle
            visible_trianlges.push_back(triangle);
        }

    }
    return visible_trianlges;
}

