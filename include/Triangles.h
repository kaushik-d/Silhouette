#pragma once

#include <vector>
#include <array>
#include <filesystem>
#include <limits>
#include <cmath>
#include <thread>

#include <Eigen/Dense>
#include "STLReader.h"

struct CoordinateHash
{
    size_t operator()(const Eigen::Vector3d &arr) const
    {
        std::hash<double> hasher;
        size_t hash = 0;
        for (double val : arr)
        {
            hash ^= hasher(val) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        }
        return hash;
    }
};

struct CoordinateEqual
{
    CoordinateEqual(double tol) : tolerance(tol) {}

    bool operator()(const Eigen::Vector3d &a, const Eigen::Vector3d &b) const
    {
        double distance = std::sqrt(
            (a[0] - b[0]) * (a[0] - b[0]) +
            (a[1] - b[1]) * (a[1] - b[1]) +
            (a[2] - b[2]) * (a[2] - b[2])

        );
        return distance <= tolerance;
    }

    double tolerance;
};

class Triangles
{

public:
    // Checks for co-incident vertex and creats a list of unique nodal coordiantes
    // convert information of triangles to a connecivity matrix, each row is three node numbers.
    void convert_trianles_to_connectivity(
        std::vector<STL::Triangle> triangles, 
        double tolerance = std::numeric_limits<double>::min(),
        unsigned int num_threads = std::thread::hardware_concurrency());

    int num_nodes() const
    {
        return m_vertex_coordinates.size();
    }

    const std::vector<std::vector<int>> &connectivity() const
    {
        return m_connectivity;
    }

    const std::vector<Eigen::Vector3d> &vertex_coordinates() const
    {
        return m_vertex_coordinates;
    }

    Triangles &operator=(const Triangles &) = delete;
    Triangles(const Triangles &) = delete;
    Triangles() = default;

private:
    std::vector<std::vector<int>> m_connectivity;            // nodal connectivity of a triagnle. Each node numbers are positions in the vertex_array of coordinates
    std::vector<Eigen::Vector3d> m_vertex_coordinates; // coordinates of vertex
};