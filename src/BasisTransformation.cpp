#include "BasisTransformation.h"

BasisTransformation::BasisTransformation(const Eigen::Vector3d &twoD_plane_normal, const Eigen::Vector3d &twoD_plane_origin)
{
    Eigen::Vector3d new_z = twoD_plane_normal.normalized();

    Eigen::Vector3d new_x(1, 0, 0);
    Eigen::Vector3d new_y(1, 0, 0);

    Eigen::Vector3d x_test(1, 0, 0);
    x_test = x_test + twoD_plane_origin;
    Eigen::Vector3d y_test(0, 1, 0);
    y_test = y_test + twoD_plane_origin;
    Eigen::Vector3d z_test(0, 0, 1);
    z_test = z_test + twoD_plane_origin;

    Eigen::Vector3d xcross = x_test.cross(new_z);
    Eigen::Vector3d ycross = y_test.cross(new_z);
    Eigen::Vector3d zcross = z_test.cross(new_z);

    const auto a = twoD_plane_normal[0];
    const auto b = twoD_plane_normal[1];
    const auto c = twoD_plane_normal[2];

    const auto d = twoD_plane_origin[0];
    const auto e = twoD_plane_origin[1];
    const auto f = twoD_plane_origin[2];

    if (std::abs(xcross.norm()) > 10E-6)
    {
        auto x = x_test[0];
        auto y = x_test[1];
        auto z = x_test[2];

        auto t = (a * d - a * x + b * e - b * y + c * f - c * z) / (a * a + b * b + c * c);

        new_x[0] = x_test[0] + t * a;
        new_x[1] = x_test[1] + t * b;
        new_x[2] = x_test[2] + t * c;

        new_x = new_x - twoD_plane_origin;
    }
    else if (std::abs(ycross.norm()) > 10E-6)
    {
        auto x = y_test[0];
        auto y = y_test[1];
        auto z = y_test[2];

        auto t = (a * d - a * x + b * e - b * y + c * f - c * z) / (a * a + b * b + c * c);

        new_x[0] = y_test[0] + t * a;
        new_x[1] = y_test[1] + t * b;
        new_x[2] = y_test[2] + t * c;

        new_x = new_x - twoD_plane_origin;
    }
    else
    {
        auto x = z_test[0];
        auto y = z_test[1];
        auto z = z_test[2];

        auto t = (a * d - a * x + b * e - b * y + c * f - c * z) / (a * a + b * b + c * c);

        new_x[0] = z_test[0] + t * a;
        new_x[1] = z_test[1] + t * b;
        new_x[2] = z_test[2] + t * c;

        new_x = new_x - twoD_plane_origin;
    }

    new_x.normalize();
    new_y = new_z.cross(new_x);

    // m_BasisForm << 1, 0, 0, 0, 1, 0, 0, 0, 1;
    m_BasisTo << new_x[0], new_x[1], new_x[2],
        new_y[0], new_y[1], new_y[2],
        new_z[0], new_z[1], new_z[2];
    m_BasisTo.transposeInPlace();

    m_Origin = twoD_plane_origin;
    // m_Origin = Eigen::Vector3d(0, 0, 0);

    m_Transformation = m_BasisTo;                      // * m_BasisForm.inverse();
    m_Transformation_inv = m_Transformation.inverse(); // computeInverseAndDetWithCheck();
}

Eigen::Vector3d BasisTransformation::transform(const Eigen::Vector3d &vector) const
{
    //return m_Transformation * (vector - m_Origin);
    return m_Transformation * vector;
}

Eigen::Vector3d BasisTransformation::reverse_transform(const Eigen::Vector3d &vector) const
{
    //return m_Transformation_inv * vector + m_Origin;
    return m_Transformation_inv * vector;
}