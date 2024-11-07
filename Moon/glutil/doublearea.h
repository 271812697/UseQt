#pragma
#include <Eigen/Dense>
template <typename DerivedV, typename DerivedF, typename DeriveddblA>
void doublearea(
	const Eigen::MatrixBase<DerivedV>& V,
	const Eigen::MatrixBase<DerivedF>& F,
	Eigen::PlainObjectBase<DeriveddblA>& dblA);
template <
	typename DerivedA,
	typename DerivedB,
	typename DerivedC,
	typename DerivedD>
void doublearea(
	const Eigen::MatrixBase<DerivedA>& A,
	const Eigen::MatrixBase<DerivedB>& B,
	const Eigen::MatrixBase<DerivedC>& C,
	Eigen::PlainObjectBase<DerivedD>& D);
/// Compute the twice the signed area of a single triangle.
///
/// @param[in] A triangle corner position
/// @param[in] B triangle corner position
/// @param[in] C triangle corner position
/// @return 2*signed area of triangle
///
/// \fileinfo
template <
	typename DerivedA,
	typename DerivedB,
	typename DerivedC>
typename DerivedA::Scalar doublearea_single(
	const Eigen::MatrixBase<DerivedA>& A,
	const Eigen::MatrixBase<DerivedB>& B,
	const Eigen::MatrixBase<DerivedC>& C);
/// Compute twice the area of each intrinsic triangle in a mesh.
///
/// @param[in] l  #F by dim list of edge lengths using
///    for triangles, columns correspond to edges 23,31,12
/// @param[in] nan_replacement  what value should be used for triangles whose given
///    edge lengths do not obey the triangle inequality. These may be very
///    wrong (e.g., [100 1 1]) or may be nearly degenerate triangles whose
///    floating point side length computation leads to breach of the triangle
///    inequality. One may wish to set this parameter to 0 if side lengths l
///    are _known_ to come from a valid embedding (e.g., some mesh (V,F)). In
///    that case, the only circumstance the triangle inequality is broken is
///    when the triangle is nearly degenerate and floating point error
///    dominates: hence replacing with zero is reasonable.
/// @param[out] dblA  #F list of triangle double areas
template <typename Derivedl, typename DeriveddblA>
void doublearea(
	const Eigen::MatrixBase<Derivedl>& l,
	const typename Derivedl::Scalar nan_replacement,
	Eigen::PlainObjectBase<DeriveddblA>& dblA);
/// \overload
///
/// \brief default behavior is to assert on NaNs and leave them in place
template <typename Derivedl, typename DeriveddblA>
void doublearea(
	const Eigen::MatrixBase<Derivedl>& l,
	Eigen::PlainObjectBase<DeriveddblA>& dblA);
/// Computes twice the area for each input quadrilateral.
///
/// @param[in] V  #V by dim list of mesh vertex positions
/// @param[in] F  #F by simplex_size list of mesh faces (must be quadrilaterals)
/// @param[out] dblA  #F list of quadrilateral double areas
///
/// \fileinfo
template <typename DerivedV, typename DerivedF, typename DeriveddblA>
void doublearea_quad(
	const Eigen::MatrixBase<DerivedV>& V,
	const Eigen::MatrixBase<DerivedF>& F,
	Eigen::PlainObjectBase<DeriveddblA>& dblA);

