#pragma once
#include <Eigen/Core>
namespace MOON {
	template <
		typename Derivedwin,
		typename Derivedmodel,
		typename Derivedproj,
		typename Derivedviewport,
		typename Derivedscene>
	void unproject(
		const Eigen::MatrixBase<Derivedwin>& win,
		const Eigen::MatrixBase<Derivedmodel>& model,
		const Eigen::MatrixBase<Derivedproj>& proj,
		const Eigen::MatrixBase<Derivedviewport>& viewport,
		Eigen::PlainObjectBase<Derivedscene>& scene);
	/// \overload
	template <typename Scalar>
	Eigen::Matrix<Scalar, 3, 1> unproject(
		const Eigen::Matrix<Scalar, 3, 1>& win,
		const Eigen::Matrix<Scalar, 4, 4>& model,
		const Eigen::Matrix<Scalar, 4, 4>& proj,
		const Eigen::Matrix<Scalar, 4, 1>& viewport);

}
