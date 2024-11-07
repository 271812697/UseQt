#pragma once
#include <vector>
#include <Eigen/Core>

template <typename DerivedX, typename DerivedY, typename DerivedI>
void sortrows(
	const Eigen::DenseBase<DerivedX>& X,
	const bool ascending,
	Eigen::PlainObjectBase<DerivedY>& Y,
	Eigen::PlainObjectBase<DerivedI>& I);
/// \overload
template <typename DerivedX, typename DerivedY>
void sortrows(
	const Eigen::DenseBase<DerivedX>& X,
	const bool ascending,
	Eigen::PlainObjectBase<DerivedY>& Y);
