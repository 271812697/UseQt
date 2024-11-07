#pragma once
#include <Eigen/Dense>

  template <typename DerivedV, typename DerivedF, typename DerivedL>
   void edge_lengths(
    const Eigen::MatrixBase<DerivedV>& V,
    const Eigen::MatrixBase<DerivedF>& F,
    Eigen::PlainObjectBase<DerivedL>& L);

