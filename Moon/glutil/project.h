#pragma once
#include <Eigen/Dense>
namespace MOON {
	template <typename Scalar>
	Eigen::Matrix<Scalar, 3, 1> project(
		const    Eigen::Matrix<Scalar, 3, 1>& obj,
		const    Eigen::Matrix<Scalar, 4, 4>& model,
		const    Eigen::Matrix<Scalar, 4, 4>& proj,
		const    Eigen::Matrix<Scalar, 4, 1>& viewport);

	template <typename DerivedV, typename DerivedM, typename DerivedN, typename DerivedO, typename DerivedP>
	void project(
		const    Eigen::MatrixBase<DerivedV>& V,
		const    Eigen::MatrixBase<DerivedM>& model,
		const    Eigen::MatrixBase<DerivedN>& proj,
		const    Eigen::MatrixBase<DerivedO>& viewport,
		Eigen::PlainObjectBase<DerivedP>& P);

}
