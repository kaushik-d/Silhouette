#pragma once

#include <vector>

#include "Triangle.h"
#include "Triangles.h"

namespace Declustering 
{
    std::vector<STL::Triangle> get_outter_shell(const Triangles& triangles);
}