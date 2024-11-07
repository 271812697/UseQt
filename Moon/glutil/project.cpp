
#include "project.h"
namespace MOON {
	template <typename Scalar>
	Eigen::Matrix<Scalar, 3, 1> project(
		const    Eigen::Matrix<Scalar, 3, 1>& obj,
		const    Eigen::Matrix<Scalar, 4, 4>& model,
		const    Eigen::Matrix<Scalar, 4, 4>& proj,
		const    Eigen::Matrix<Scalar, 4, 1>& viewport)
	{
		Eigen::Matrix<Scalar, 4, 1> tmp;
		tmp << obj, 1;

		tmp = model * tmp;

		tmp = proj * tmp;

		tmp = tmp.array() / tmp(3);
		tmp = tmp.array() * 0.5f + 0.5f;
		tmp(0) = tmp(0) * viewport(2) + viewport(0);
		tmp(1) = tmp(1) * viewport(3) + viewport(1);

		return tmp.head(3);
	}

	template <typename DerivedV, typename DerivedM, typename DerivedN, typename DerivedO, typename DerivedP>
	void project(
		const    Eigen::MatrixBase<DerivedV>& V,
		const    Eigen::MatrixBase<DerivedM>& model,
		const    Eigen::MatrixBase<DerivedN>& proj,
		const    Eigen::MatrixBase<DerivedO>& viewport,
		Eigen::PlainObjectBase<DerivedP>& P)
	{
		typedef typename DerivedP::Scalar PScalar;
		Eigen::Matrix<PScalar, DerivedV::RowsAtCompileTime, 4> HV(V.rows(), 4);
		HV.leftCols(3) = V.template cast<PScalar>();
		HV.col(3).setConstant(1);
		HV = (HV * model.template cast<PScalar>().transpose() *
			proj.template cast<PScalar>().transpose()).eval();
		HV = (HV.array().colwise() / HV.col(3).array()).eval();
		HV = (HV.array() * 0.5 + 0.5).eval();
		HV.col(0) = (HV.array().col(0) * viewport(2) + viewport(0)).eval();
		HV.col(1) = (HV.array().col(1) * viewport(3) + viewport(1)).eval();
		P = HV.leftCols(3);
	}


	template void project<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<float, 4, 4, 0, 4, 4>, Eigen::Matrix<float, 4, 4, 0, 4, 4>, Eigen::Matrix<float, 4, 1, 0, 4, 1>, Eigen::Matrix<double, -1, -1, 0, -1, -1> >(Eigen::MatrixBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::MatrixBase<Eigen::Matrix<float, 4, 4, 0, 4, 4> > const&, Eigen::MatrixBase<Eigen::Matrix<float, 4, 4, 0, 4, 4> > const&, Eigen::MatrixBase<Eigen::Matrix<float, 4, 1, 0, 4, 1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> >&);
	// Explicit template instantiation
	template Eigen::Matrix<double, 3, 1, 0, 3, 1> project<double>(Eigen::Matrix<double, 3, 1, 0, 3, 1> const&, Eigen::Matrix<double, 4, 4, 0, 4, 4> const&, Eigen::Matrix<double, 4, 4, 0, 4, 4> const&, Eigen::Matrix<double, 4, 1, 0, 4, 1> const&);
	template Eigen::Matrix<float, 3, 1, 0, 3, 1> project<float>(Eigen::Matrix<float, 3, 1, 0, 3, 1> const&, Eigen::Matrix<float, 4, 4, 0, 4, 4> const&, Eigen::Matrix<float, 4, 4, 0, 4, 4> const&, Eigen::Matrix<float, 4, 1, 0, 4, 1> const&);
	template void project<Eigen::Matrix<float, -1, -1, 0, -1, -1>, Eigen::Matrix<float, 4, 4, 0, 4, 4>, Eigen::Matrix<float, 4, 4, 0, 4, 4>, Eigen::Matrix<float, 4, 1, 0, 4, 1>, Eigen::Matrix<float, -1, -1, 0, -1, -1>>(const Eigen::MatrixBase<Eigen::Matrix<float, -1, -1, 0, -1, -1>>&, const Eigen::MatrixBase<Eigen::Matrix<float, 4, 4, 0, 4, 4>>&, const Eigen::MatrixBase<Eigen::Matrix<float, 4, 4, 0, 4, 4>>&, const Eigen::MatrixBase<Eigen::Matrix<float, 4, 1, 0, 4, 1>>&, Eigen::PlainObjectBase<Eigen::Matrix<float, -1, -1, 0, -1, -1>>&);
	template void project<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<double, 4, 4, 0, 4, 4>, Eigen::Matrix<double, 4, 4, 0, 4, 4>, Eigen::Matrix<double, 4, 1, 0, 4, 1>, Eigen::Matrix<double, -1, -1, 0, -1, -1>>(const Eigen::MatrixBase<Eigen::Matrix<double, -1, -1, 0, -1, -1>>&, const Eigen::MatrixBase<Eigen::Matrix<double, 4, 4, 0, 4, 4>>&, const Eigen::MatrixBase<Eigen::Matrix<double, 4, 4, 0, 4, 4>>&, const Eigen::MatrixBase<Eigen::Matrix<double, 4, 1, 0, 4, 1>>&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1>>&);
	template void project<Eigen::Matrix<double, 3, 1, 0, 3, 1>, Eigen::Matrix<float, 4, 4, 0, 4, 4>, Eigen::Matrix<float, 4, 4, 0, 4, 4>, Eigen::Matrix<float, 4, 1, 0, 4, 1>, Eigen::Matrix<double, 3, 1, 0, 3, 1> >(Eigen::MatrixBase<Eigen::Matrix<double, 3, 1, 0, 3, 1> > const&, Eigen::MatrixBase<Eigen::Matrix<float, 4, 4, 0, 4, 4> > const&, Eigen::MatrixBase<Eigen::Matrix<float, 4, 4, 0, 4, 4> > const&, Eigen::MatrixBase<Eigen::Matrix<float, 4, 1, 0, 4, 1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, 3, 1, 0, 3, 1> >&);
	template void project<Eigen::Matrix<double, 1, 3, 1, 1, 3>, Eigen::Matrix<float, 4, 4, 0, 4, 4>, Eigen::Matrix<float, 4, 4, 0, 4, 4>, Eigen::Matrix<float, 4, 1, 0, 4, 1>, Eigen::Matrix<double, 1, 3, 1, 1, 3> >(Eigen::MatrixBase<Eigen::Matrix<double, 1, 3, 1, 1, 3> > const&, Eigen::MatrixBase<Eigen::Matrix<float, 4, 4, 0, 4, 4> > const&, Eigen::MatrixBase<Eigen::Matrix<float, 4, 4, 0, 4, 4> > const&, Eigen::MatrixBase<Eigen::Matrix<float, 4, 1, 0, 4, 1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, 1, 3, 1, 1, 3> >&);
}