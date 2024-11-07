#pragma once
#include <Eigen/Core>

  /// Compute internal angles for a triangle mesh
  ///
  /// @param[in] V  #V by dim eigen Matrix of mesh vertex nD positions
  /// @param[in] F  #F by poly-size eigen Matrix of face (triangle) indices
  /// @param[out] K  #F by poly-size eigen Matrix of internal angles
  ///     for triangles, columns correspond to edges [1,2],[2,0],[0,1]
  ///
  /// \note if poly-size ≠ 3 then dim must equal 3.
  template <typename DerivedV, typename DerivedF, typename DerivedK>
  void internal_angles(
    const Eigen::MatrixBase<DerivedV>& V,
    const Eigen::MatrixBase<DerivedF>& F,
    Eigen::PlainObjectBase<DerivedK> & K);


