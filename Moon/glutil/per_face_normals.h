#pragma once
#include <Eigen/Core>

template <typename DerivedV, typename DerivedF, typename DerivedZ, typename DerivedN>
void per_face_normals(
	const Eigen::MatrixBase<DerivedV>& V,
	const Eigen::MatrixBase<DerivedF>& F,
	const Eigen::MatrixBase<DerivedZ>& Z,
	Eigen::PlainObjectBase<DerivedN>& N);
/// \overload
/// \brief Wrapper with Z = (0,0,0). Note that this means that row norms will be zero
/// (i.e. not 1) for degenerate normals.
template <typename DerivedV, typename DerivedF, typename DerivedN>
void per_face_normals(
	const Eigen::MatrixBase<DerivedV>& V,
	const Eigen::MatrixBase<DerivedF>& F,
	Eigen::PlainObjectBase<DerivedN>& N);
/// \overload
/// \brief Special version where order of face indices is guaranteed not to effect
/// output.
template <typename DerivedV, typename DerivedF, typename DerivedN>
void per_face_normals_stable(
	const Eigen::MatrixBase<DerivedV>& V,
	const Eigen::MatrixBase<DerivedF>& F,
	Eigen::PlainObjectBase<DerivedN>& N);
/// Per face normals for a general polygon mesh
/// @param[in] V  #V by 3 list of mesh vertex positions
/// @param[in] I  #I vectorized list of polygon corner indices into rows of some matrix V
/// @param[in] C  #polygons+1 list of cumulative polygon sizes so that C(i+1)-C(i) = size of
///     the ith polygon, and so I(C(i)) through I(C(i+1)-1) are the indices of
///     the ith polygon
/// @param[in] corner_threshold  threshold in degrees on sharp angles
/// @param[out] N  #polygons by 3 list of per face normals
/// @param[out] VV  #I+#polygons by 3 list of auxiliary triangle mesh vertex positions
/// @param[out] FF  #I by 3 list of triangle indices into rows of VV
/// @param[out] J  #I list of indices into original polygons
template <
	typename DerivedV,
	typename DerivedI,
	typename DerivedC,
	typename DerivedN,
	typename DerivedVV,
	typename DerivedFF,
	typename DerivedJ>
void per_face_normals(
	const Eigen::MatrixBase<DerivedV>& V,
	const Eigen::MatrixBase<DerivedI>& I,
	const Eigen::MatrixBase<DerivedC>& C,
	Eigen::PlainObjectBase<DerivedN>& N,
	Eigen::PlainObjectBase<DerivedVV>& VV,
	Eigen::PlainObjectBase<DerivedFF>& FF,
	Eigen::PlainObjectBase<DerivedJ>& J);

