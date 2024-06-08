#pragma once
#include <array>
#include <vector>
#include <filesystem>
#include "Triangle.h"

namespace STL
{
    std::vector<STL::Triangle> readBinary(const std::filesystem::path& filename);
    void writeBinary(const std::filesystem::path& filename, const std::vector<STL::Triangle>& triangles);

    void check_normals(std::vector<STL::Triangle> &triangles);

}