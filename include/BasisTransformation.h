#pragma once
#include <Eigen/Dense>

class BasisTransformation
{
public:
    //BasisTransformation(const Eigen::MatrixXd &BasisForm, const Eigen::MatrixXd &BasisTo);
    BasisTransformation(const Eigen::Vector3d &twoD_plane_normal, const Eigen::Vector3d &twoD_plane_origin);

    Eigen::Vector3d transform(const Eigen::Vector3d &vector) const;
    Eigen::Vector3d reverse_transform(const Eigen::Vector3d &vector) const;

private:
    Eigen::Matrix3d m_BasisForm{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    Eigen::Matrix3d m_Transformation; //{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    Eigen::Matrix3d m_BasisTo; //{{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
    Eigen::Vector3d m_Origin; //{0, 0, 0};
};