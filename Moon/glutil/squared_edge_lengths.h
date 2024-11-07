#pragma
#include <Eigen/Dense>
template <typename DerivedV, typename DerivedF, typename DerivedL>
void squared_edge_lengths(
	const Eigen::MatrixBase<DerivedV>& V,
	const Eigen::MatrixBase<DerivedF>& F,
	Eigen::PlainObjectBase<DerivedL>& L);

