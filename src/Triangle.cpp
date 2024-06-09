#include <iostream>
#include "Triangle.h"

void STL::Triangle::project(const Eigen::Vector3d &shadow_plane_normal, const Eigen::Vector3d &shadow_plane_origin)
{
    //normal = shadow_plane_normal;

    for (auto &vertex : vertex_list)
    {
        Eigen::Vector3d v = vertex - shadow_plane_origin;
        double dist = v.dot(shadow_plane_normal);
        vertex = vertex - dist * shadow_plane_normal;
        /*
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
        */
    }
}

void STL::Triangle::transform(const BasisTransformation &transformation)
{
    for (auto &vertex : vertex_list)
    {
        auto vert1 = transformation.transform(vertex);
        vertex = vert1;
    }
}

polygon_t STL::Triangle::toPolygon2D(const BasisTransformation &transformation) const
{
    // only valid after projection

    auto vert1 = transformation.transform(vertex_list[0]);
    auto vert2 = transformation.transform(vertex_list[1]);
    auto vert3 = transformation.transform(vertex_list[2]);

    polygon_t poly;
    bg::append(poly.outer(), point_t(vert3[0], vert3[1]));
    bg::append(poly.outer(), point_t(vert2[0], vert2[1]));
    bg::append(poly.outer(), point_t(vert1[0], vert1[1]));
    bg::append(poly.outer(), point_t(vert3[0], vert3[1]));

    double area = bg::area(poly);

    if(area < 0.0)
    {
        polygon_t poly1;
        bg::append(poly1.outer(), point_t(vert1[0], vert1[1]));
        bg::append(poly1.outer(), point_t(vert2[0], vert2[1]));
        bg::append(poly1.outer(), point_t(vert3[0], vert3[1]));
        bg::append(poly1.outer(), point_t(vert1[0], vert1[1]));

        double area1 = bg::area(poly1);

        std::cout << "Area corrected " << area << " -> " << area1 << std::endl;

        //bg::correct(poly1);
        return poly1;
    }

    //bg::correct(poly);
    return poly;
}