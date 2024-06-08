#pragma once

#include <vector>

#include "Triangle.h"

namespace FindVisible
{
    std::vector<STL::Triangle> get_visible_list(const std::vector<STL::Triangle>& triangles, const Eigen::Vector3d &shadow_plane_normal);
    void project_tringles(std::vector<STL::Triangle> &triangles, const Eigen::Vector3d &shadow_plane_normal, const Eigen::Vector3d &shadow_plane_origin);
    mpolygon_t get_shadow(const std::vector<STL::Triangle>& triangles, const Eigen::Vector3d& shadow_plane_normal, const Eigen::Vector3d &shadow_plane_origin = Eigen::Vector3d(0, 0, 0));
}