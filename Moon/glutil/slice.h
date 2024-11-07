#pragma once
#include <Eigen/Sparse>
#include <vector>

template <
	typename TX,
	typename TY,
	typename DerivedR,
	typename DerivedC>
void slice(
	const Eigen::SparseMatrix<TX>& X,
	const Eigen::DenseBase<DerivedR>& R,
	const Eigen::DenseBase<DerivedC>& C,
	Eigen::SparseMatrix<TY>& Y);
template <
	typename MatX,
	typename DerivedR,
	typename MatY>
void slice(
	const MatX& X,
	const Eigen::DenseBase<DerivedR>& R,
	const int dim,
	MatY& Y);
template< class T >
void slice(
	const std::vector<T>& X,
	std::vector<size_t> const& R,
	std::vector<T>& Y);
template <typename DerivedX, typename DerivedY, typename DerivedR>
void slice(
	const Eigen::DenseBase<DerivedX>& X,
	const Eigen::DenseBase<DerivedR>& R,
	Eigen::PlainObjectBase<DerivedY>& Y);

template <
	typename DerivedX,
	typename DerivedR,
	typename DerivedC,
	typename DerivedY>
void slice(
	const Eigen::DenseBase<DerivedX>& X,
	const Eigen::DenseBase<DerivedR>& R,
	const Eigen::DenseBase<DerivedC>& C,
	Eigen::PlainObjectBase<DerivedY>& Y);
template <typename DerivedX, typename DerivedR>
DerivedX slice(
	const Eigen::DenseBase<DerivedX>& X,
	const Eigen::DenseBase<DerivedR>& R);
/// \overload
template <typename DerivedX, typename DerivedR>
DerivedX slice(
	const Eigen::DenseBase<DerivedX>& X,
	const Eigen::DenseBase<DerivedR>& R,
	const int dim);

