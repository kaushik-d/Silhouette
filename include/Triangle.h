#pragma once

#include <Eigen/Dense>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/geometries.hpp>

#include "BasisTransformation.h"

namespace bg = boost::geometry;

// typedef Eigen::Vector3d point_t;

typedef bg::model::point<double, 2, bg::cs::cartesian> point_t;
typedef bg::model::polygon<point_t> polygon_t;
typedef bg::model::multi_polygon<polygon_t> mpolygon_t;

namespace STL
{
    struct Triangle
    {
        Eigen::Vector3d normal;
        std::array<Eigen::Vector3d, 3> vertex_list;

        void project(const Eigen::Vector3d &shadow_plane_normal, const Eigen::Vector3d &shadow_plane_origin = Eigen::Vector3d(0, 0, 0));

        polygon_t toPolygon2D(const BasisTransformation& transformation) const;
    };

}