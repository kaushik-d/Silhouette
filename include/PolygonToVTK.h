#pragma once

#include <iostream>
#include "Triangle.h"

#include "vtu11/vtu11.hpp"


namespace vtk_utils
{
    void export_polygon(const mpolygon_t& multi_polygon, const Eigen::Vector3d& shadow_plane_normal, const Eigen::Vector3d &shadow_plane_origin, const std::filesystem::path& finame_name = "shadow.vtu");
}