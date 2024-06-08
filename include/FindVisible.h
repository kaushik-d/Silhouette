#pragma once

#include <vector>

#include "Triangle.h"

namespace FindVisible
{
    std::vector<STL::Triangle> get_visible_list(const std::vector<STL::Triangle>& triangles);

    polygon_t get_shadow(const std::vector<STL::Triangle>& triangles);
}