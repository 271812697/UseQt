#pragma once
#include <Eigen/Core>


enum PerVertexNormalsWeightingType
{
	/// Incident face normals have uniform influence on vertex normal
	PER_VERTEX_NORMALS_WEIGHTING_TYPE_UNIFORM = 0,
	/// Incident face normals are averaged weighted by area
	PER_VERTEX_NORMALS_WEIGHTING_TYPE_AREA = 1,
	/// Incident face normals are averaged weighted by incident angle of vertex
	PER_VERTEX_NORMALS_WEIGHTING_TYPE_ANGLE = 2,
	/// Area weights
	PER_VERTEX_NORMALS_WEIGHTING_TYPE_DEFAULT = 3,
	/// Total number of weighting types
	NUM_PER_VERTEX_NORMALS_WEIGHTING_TYPE = 4
};

template <
	typename DerivedV,
	typename DerivedF,
	typename DerivedN>
void per_vertex_normals(
	const Eigen::MatrixBase<DerivedV>& V,
	const Eigen::MatrixBase<DerivedF>& F,
	const PerVertexNormalsWeightingType weighting,
	Eigen::PlainObjectBase<DerivedN>& N);
/// \overload
template <
	typename DerivedV,
	typename DerivedF,
	typename DerivedN>
void per_vertex_normals(
	const Eigen::MatrixBase<DerivedV>& V,
	const Eigen::MatrixBase<DerivedF>& F,
	Eigen::PlainObjectBase<DerivedN>& N);
/// \overload
///
/// @param[in] FN  #F by 3 matrix of face (triangle) normals
template <typename DerivedV, typename DerivedF, typename DerivedFN, typename DerivedN>
void per_vertex_normals(
	const Eigen::MatrixBase<DerivedV>& V,
	const Eigen::MatrixBase<DerivedF>& F,
	const PerVertexNormalsWeightingType weighting,
	const Eigen::MatrixBase<DerivedFN>& FN,
	Eigen::PlainObjectBase<DerivedN>& N);
/// \overload
template <
	typename DerivedV,
	typename DerivedF,
	typename DerivedFN,
	typename DerivedN>
void per_vertex_normals(
	const Eigen::MatrixBase<DerivedV>& V,
	const Eigen::MatrixBase<DerivedF>& F,
	const Eigen::MatrixBase<DerivedFN>& FN,
	Eigen::PlainObjectBase<DerivedN>& N);



