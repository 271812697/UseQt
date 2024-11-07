#include "unproject.h"

#include <Eigen/Dense>
#include <Eigen/LU>
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
		Eigen::PlainObjectBase<Derivedscene>& scene)
	{
		if (win.cols() != 3)
		{
			assert(win.rows() == 3);
			// needless transposes
			Eigen::Matrix<typename Derivedscene::Scalar, 1, 3> sceneT;
			unproject(win.transpose().eval(), model, proj, viewport, sceneT);
			scene = sceneT.head(3);
			return;
		}
		assert(win.cols() == 3);
		const int n = win.rows();
		scene.resize(n, 3);
		for (int i = 0; i < n; i++)
		{
			typedef typename Derivedscene::Scalar Scalar;
			Eigen::Matrix<Scalar, 4, 4> Inverse =
				(proj.template cast<Scalar>() * model.template cast<Scalar>()).inverse();

			Eigen::Matrix<Scalar, 4, 1> tmp;
			tmp << win.row(i).head(3).transpose(), 1;
			tmp(0) = (tmp(0) - viewport(0, 0)) / viewport(2, 0);
			tmp(1) = (tmp(1) - viewport(1, 0)) / viewport(3, 0);
			tmp = tmp.array() * 2.0f - 1.0f;

			Eigen::Matrix<Scalar, 4, 1> obj = Inverse * tmp;
			obj /= obj(3);

			scene.row(i).head(3) = obj.head(3);
		}
	}

	template <typename Scalar>
	Eigen::Matrix<Scalar, 3, 1> unproject(
		const    Eigen::Matrix<Scalar, 3, 1>& win,
		const    Eigen::Matrix<Scalar, 4, 4>& model,
		const    Eigen::Matrix<Scalar, 4, 4>& proj,
		const    Eigen::Matrix<Scalar, 4, 1>& viewport)
	{
		Eigen::Matrix<Scalar, 3, 1> scene;
		unproject(win, model, proj, viewport, scene);
		return scene;
	}


	template Eigen::Matrix<float, 3, 1, 0, 3, 1> unproject<float>(Eigen::Matrix<float, 3, 1, 0, 3, 1> const&, Eigen::Matrix<float, 4, 4, 0, 4, 4> const&, Eigen::Matrix<float, 4, 4, 0, 4, 4> const&, Eigen::Matrix<float, 4, 1, 0, 4, 1> const&);
	template Eigen::Matrix<double, 3, 1, 0, 3, 1> unproject<double>(Eigen::Matrix<double, 3, 1, 0, 3, 1> const&, Eigen::Matrix<double, 4, 4, 0, 4, 4> const&, Eigen::Matrix<double, 4, 4, 0, 4, 4> const&, Eigen::Matrix<double, 4, 1, 0, 4, 1> const&);
	template void unproject<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<float, 4, 4, 0, 4, 4>, Eigen::Matrix<float, 4, 4, 0, 4, 4>, Eigen::Matrix<float, 4, 1, 0, 4, 1>, Eigen::Matrix<double, -1, -1, 0, -1, -1> >(Eigen::MatrixBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::MatrixBase<Eigen::Matrix<float, 4, 4, 0, 4, 4> > const&, Eigen::MatrixBase<Eigen::Matrix<float, 4, 4, 0, 4, 4> > const&, Eigen::MatrixBase<Eigen::Matrix<float, 4, 1, 0, 4, 1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> >&);
	template void unproject<Eigen::Matrix<float, 3, 1, 0, 3, 1>, Eigen::Matrix<float, 4, 4, 0, 4, 4>, Eigen::Matrix<float, 4, 4, 0, 4, 4>, Eigen::Matrix<float, 4, 1, 0, 4, 1>, Eigen::Matrix<float, 3, 1, 0, 3, 1> >(Eigen::MatrixBase<Eigen::Matrix<float, 3, 1, 0, 3, 1> > const&, Eigen::MatrixBase<Eigen::Matrix<float, 4, 4, 0, 4, 4> > const&, Eigen::MatrixBase<Eigen::Matrix<float, 4, 4, 0, 4, 4> > const&, Eigen::MatrixBase<Eigen::Matrix<float, 4, 1, 0, 4, 1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<float, 3, 1, 0, 3, 1> >&);
	template void unproject<Eigen::Matrix<double, 3, 1, 0, 3, 1>, Eigen::Matrix<float, 4, 4, 0, 4, 4>, Eigen::Matrix<float, 4, 4, 0, 4, 4>, Eigen::Matrix<float, 4, 1, 0, 4, 1>, Eigen::Matrix<double, 3, 1, 0, 3, 1> >(Eigen::MatrixBase<Eigen::Matrix<double, 3, 1, 0, 3, 1> > const&, Eigen::MatrixBase<Eigen::Matrix<float, 4, 4, 0, 4, 4> > const&, Eigen::MatrixBase<Eigen::Matrix<float, 4, 4, 0, 4, 4> > const&, Eigen::MatrixBase<Eigen::Matrix<float, 4, 1, 0, 4, 1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, 3, 1, 0, 3, 1> >&);
	template void unproject<Eigen::Matrix<float, 1, 3, 1, 1, 3>, Eigen::Matrix<float, 4, 4, 0, 4, 4>, Eigen::Matrix<float, 4, 4, 0, 4, 4>, Eigen::Matrix<float, 4, 1, 0, 4, 1>, Eigen::Matrix<float, 1, 3, 1, 1, 3> >(Eigen::MatrixBase<Eigen::Matrix<float, 1, 3, 1, 1, 3> > const&, Eigen::MatrixBase<Eigen::Matrix<float, 4, 4, 0, 4, 4> > const&, Eigen::MatrixBase<Eigen::Matrix<float, 4, 4, 0, 4, 4> > const&, Eigen::MatrixBase<Eigen::Matrix<float, 4, 1, 0, 4, 1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<float, 1, 3, 1, 1, 3> >&);


}
