#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <algorithm>

#include "STLReader.h"

std::vector<STL::Triangle> STL::readBinary(const std::filesystem::path &filename, const BasisTransformation& transformation)
{
    std::vector<STL::Triangle> triangles;
    std::ifstream file(filename, std::ios::binary);

    if (!file)
    {
        throw std::runtime_error("Error opening file: " + filename.string());
        return triangles;
    }

    // Skip the 80-byte header
    file.ignore(80);

    uint32_t numTriangles;
    file.read(reinterpret_cast<char *>(&numTriangles), 4);

    triangles.reserve(numTriangles);

    for (uint32_t i = 0; i < numTriangles; ++i)
    {
        STL::Triangle triangle;
        float normal[3];
        float vertex1[3];
        float vertex2[3];
        float vertex3[3];
        uint16_t attributeByteCount;

        file.read(reinterpret_cast<char *>(normal), 12);
        triangle.normal = {normal[0], normal[1], normal[2]};
        triangle.normal = transformation.transform(triangle.normal);

        file.read(reinterpret_cast<char *>(vertex1), 12);
        triangle.vertex_list[0] = {vertex1[0], vertex1[1], vertex1[2]};
        triangle.vertex_list[0] = transformation.transform(triangle.vertex_list[0]);

        file.read(reinterpret_cast<char *>(vertex2), 12);
        triangle.vertex_list[1] = {vertex2[0], vertex2[1], vertex2[2]};
        triangle.vertex_list[1] = transformation.transform(triangle.vertex_list[1]);

        file.read(reinterpret_cast<char *>(vertex3), 12);
        triangle.vertex_list[2] = {vertex3[0], vertex3[1], vertex3[2]};
        triangle.vertex_list[2] = transformation.transform(triangle.vertex_list[2]);

        file.read(reinterpret_cast<char *>(&attributeByteCount), 2);

        triangles.push_back(triangle);
    }

    file.close();

    return triangles;
}

void STL::check_normals(std::vector<STL::Triangle> &triangles)
{
    int n_normals_set(0), n_normals_corrected(0);

    for (auto &triangle : triangles)
    {
        // assumeing right hand orientation

        Eigen::Vector3d edge1(triangle.vertex_list[1] - triangle.vertex_list[0]);
        Eigen::Vector3d edge2(triangle.vertex_list[2] - triangle.vertex_list[0]);

        auto normal = edge1.cross(edge2);
        normal.normalize();

        triangle.normal.normalize();

        if (triangle.normal.dot(triangle.normal) < 10e-6)
        {
            n_normals_set++;
            triangle.normal = normal;
        }
        else if (std::abs(triangle.normal.dot(normal) - 1.0) > 10e-6)
        {
            n_normals_corrected++;
            std::reverse(triangle.vertex_list.begin(), triangle.vertex_list.end());

            Eigen::Vector3d edge1(triangle.vertex_list[1] - triangle.vertex_list[0]);
            Eigen::Vector3d edge2(triangle.vertex_list[2] - triangle.vertex_list[0]);

            auto normal = edge1.cross(edge2);
            normal.normalize();

            if (std::abs(triangle.normal.dot(normal) - 1.0) > 10e-6)
            {
                std::runtime_error("Error in setting normals.");
            }
        }
    }

    std::cout << std::endl;
    std::cout << " Number of normals set " << n_normals_set << std::endl;
    std::cout << " Number of normals corrected " << n_normals_corrected << std::endl;
}

void STL::writeBinary(const std::filesystem::path &filename, const std::vector<STL::Triangle> &triangles)
{
    std::ofstream file(filename, std::ios::binary);

    if (!file)
    {
        throw std::runtime_error("Error opening file: " + filename.string());
        return;
    }

    // Write the 80-byte header
    char header[80] = "Binary STL file";
    file.write(header, 80);

    uint32_t numTriangles = static_cast<uint32_t>(triangles.size());
    file.write(reinterpret_cast<char *>(&numTriangles), 4);

    for (const auto &triangle : triangles)
    {
        float normal[3] = {static_cast<float>(triangle.normal[0]), static_cast<float>(triangle.normal[1]), static_cast<float>(triangle.normal[2])};
        float vertex1[3] = {static_cast<float>(triangle.vertex_list[0][0]), static_cast<float>(triangle.vertex_list[0][1]), static_cast<float>(triangle.vertex_list[0][2])};
        float vertex2[3] = {static_cast<float>(triangle.vertex_list[1][0]), static_cast<float>(triangle.vertex_list[1][1]), static_cast<float>(triangle.vertex_list[1][2])};
        float vertex3[3] = {static_cast<float>(triangle.vertex_list[2][0]), static_cast<float>(triangle.vertex_list[2][1]), static_cast<float>(triangle.vertex_list[2][2])};
        uint16_t attributeByteCount = 0;

        file.write(reinterpret_cast<char *>(normal), 12);

        file.write(reinterpret_cast<char *>(vertex1), 12);

        file.write(reinterpret_cast<char *>(vertex2), 12);

        file.write(reinterpret_cast<char *>(vertex3), 12);

        file.write(reinterpret_cast<char *>(&attributeByteCount), 2);
    }

    file.close();
}
