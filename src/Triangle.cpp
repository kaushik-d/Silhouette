#include "Triangle.h"

void STL::Triangle::project(const Eigen::Vector3d &shadow_plane_normal, const Eigen::Vector3d &shadow_plane_origin)
{
    normal = shadow_plane_normal;

    for (auto &vertex : vertex_list)
    {

        auto a = normal[0];
        auto b = normal[1];
        auto c = normal[2];

        auto d = shadow_plane_origin[0];
        auto e = shadow_plane_origin[1];
        auto f = shadow_plane_origin[2];

        auto x = vertex[0];
        auto y = vertex[1];
        auto z = vertex[2];

        auto t = (a * d - a * x + b * e - b * y + c * f - c * z) / (a * a + b * b + c * c);

        vertex[0] = vertex[0] + t * a;
        vertex[1] = vertex[1] + t * b;
        vertex[2] = vertex[2] + t * c;
    }
}