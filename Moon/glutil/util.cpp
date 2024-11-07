#include "util.h"
#include <glad/glad.h>
#include <Eigen/Geometry>
#include <iostream>
#include <cstdio>

static unsigned int load_shader(
	const std::string& src, const GLenum type)
{
	if (src.empty())
	{
		return 0;
	}

	unsigned int s = glCreateShader(type);
	if (s == 0)
	{
		fprintf(stderr, "Error: load_shader() failed to create shader.\n");
		return 0;
	}

	const char* c = src.c_str();
	glShaderSource(s, 1, &c, NULL);
	glCompileShader(s);
	return s;
}
bool MOON::create_shader_program(
	const std::string& geom_source,
	const std::string& vert_source,
	const std::string& frag_source,
	const std::map<std::string, unsigned int>& attrib,
	unsigned int& id)
{
	using namespace std;
	if (vert_source == "" && frag_source == "")
	{
		cerr <<
			"create_shader_program() could not create shader program,"
			" both .vert and .frag source given were empty" << endl;
		return false;
	}

	// create program
	id = glCreateProgram();
	if (id == 0)
	{
		cerr << "create_shader_program() could not create shader program." << endl;
		return false;
	}
	GLuint g = 0, f = 0, v = 0;

	if (geom_source != "")
	{
		// load vertex shader
		g = load_shader(geom_source.c_str(), GL_GEOMETRY_SHADER);
		if (g == 0)
		{
			cerr << "geometry shader failed to compile." << endl;
			return false;
		}
		glAttachShader(id, g);
	}

	if (vert_source != "")
	{
		// load vertex shader
		v = load_shader(vert_source.c_str(), GL_VERTEX_SHADER);
		if (v == 0)
		{
			cerr << "vertex shader failed to compile." << endl;
			return false;
		}
		glAttachShader(id, v);
	}

	if (frag_source != "")
	{
		// load fragment shader
		f = load_shader(frag_source.c_str(), GL_FRAGMENT_SHADER);
		if (f == 0)
		{
			cerr << "fragment shader failed to compile." << endl;
			return false;
		}
		glAttachShader(id, f);
	}

	// loop over attributes
	for (
		std::map<std::string, unsigned int>::const_iterator ait = attrib.begin();
		ait != attrib.end();
		ait++)
	{
		glBindAttribLocation(
			id,
			(*ait).second,
			(*ait).first.c_str());
	}
	// Link program
	glLinkProgram(id);
	const auto& detach = [&id](const unsigned int shader)
		{
			if (shader)
			{
				glDetachShader(id, shader);
				glDeleteShader(shader);
			}
		};
	detach(g);
	detach(f);
	detach(v);

	// print log if any
	//MOON::print_program_info_log(id);

	return true;
}



bool MOON::create_shader_program(
	const std::string& vert_source,
	const std::string& frag_source,
	const std::map<std::string, unsigned int>& attrib,
	GLuint& prog_id)
{
	return create_shader_program("", vert_source, frag_source, attrib, prog_id);
}
GLuint MOON::create_shader_program(
	const std::string& geom_source,
	const std::string& vert_source,
	const std::string& frag_source,
	const std::map<std::string, unsigned int>& attrib)
{
	GLuint prog_id = 0;
	create_shader_program(geom_source, vert_source, frag_source, attrib, prog_id);
	return prog_id;
}

GLuint MOON::create_shader_program(
	const std::string& vert_source,
	const std::string& frag_source,
	const std::map<std::string, unsigned int>& attrib)
{
	GLuint prog_id = 0;
	create_shader_program(vert_source, frag_source, attrib, prog_id);
	return prog_id;
}

namespace MOON {
	template <typename Scalar>
	void bind_vertex_attrib_array_helper(
		const GLint id,
		const int size,
		const int num_cols,
		const Scalar* data,
		const bool refresh);

	template <>
	void bind_vertex_attrib_array_helper<float>(
		const GLint id,
		const int size,
		const int num_cols,
		const float* data,
		const bool refresh)
	{
		if (refresh)
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size, data, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(id, num_cols, GL_FLOAT, GL_FALSE, 0, 0);
	};

	template <>
	void bind_vertex_attrib_array_helper<double>(
		const GLint id,
		const int size,
		const int num_cols,
		const double* data,
		const bool refresh)
	{
		// Are you really sure you want to use doubles? Are you going to change the
		// `in vec3` etc. in your vertex shader to `in dvec3` ?
		// Are you on a mac, where this will be emulated in software?
		if (refresh)
			glBufferData(GL_ARRAY_BUFFER, sizeof(double) * size, data, GL_DYNAMIC_DRAW);
		glVertexAttribLPointer(id, num_cols, GL_DOUBLE, 0, 0);
	};
}


namespace MOON {
	template <typename Scalar, int RowsAtCompileTime, int ColsAtCompileTime>
	GLint bind_vertex_attrib_array(
		const GLuint program_shader,
		const std::string& name,
		GLuint bufferID,
		const Eigen::Matrix<Scalar, RowsAtCompileTime, ColsAtCompileTime, Eigen::RowMajor>& M,
		const bool refresh)
	{
		GLint id = glGetAttribLocation(program_shader, name.c_str());
		if (id < 0)
			return id;
		if (M.size() == 0)
		{
			glDisableVertexAttribArray(id);
			return id;
		}
		glBindBuffer(GL_ARRAY_BUFFER, bufferID);

		bind_vertex_attrib_array_helper<Scalar>(
			id, M.size(), M.cols(), M.data(), refresh);

		glEnableVertexAttribArray(id);
		return id;
	}
	template <
		typename DerivedV,
		typename DerivedF,
		typename DerivedBC>
	void barycenter(
		const Eigen::MatrixBase<DerivedV>& V,
		const Eigen::MatrixBase<DerivedF>& F,
		Eigen::PlainObjectBase<DerivedBC>& BC)
	{
		BC.setZero(F.rows(), V.cols());
		// Loop over faces
		for (int i = 0; i < F.rows(); i++)
		{
			// loop around face
			for (int j = 0; j < F.cols(); j++)
			{
				// Accumulate
				BC.row(i) += V.row(F(i, j));
			}
			// average
			BC.row(i) /= double(F.cols());
		}
	}


	// Explicit template instantiation
	// generated by autoexplicit.sh
	// generated by autoexplicit.sh
	template void barycenter<Eigen::Matrix<double, -1, 2, 0, -1, 2>, Eigen::Matrix<int, -1, 2, 0, -1, 2>, Eigen::Matrix<double, -1, 2, 0, -1, 2> >(Eigen::MatrixBase<Eigen::Matrix<double, -1, 2, 0, -1, 2> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, 2, 0, -1, 2> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 2, 0, -1, 2> >&);
	// generated by autoexplicit.sh
	template void barycenter<Eigen::Matrix<double, -1, 3, 0, -1, 3>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<double, -1, -1, 0, -1, -1> >(Eigen::MatrixBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> >&);
	// generated by autoexplicit.sh
	template void barycenter<Eigen::Matrix<float, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<float, -1, 3, 0, -1, 3> >(Eigen::MatrixBase<Eigen::Matrix<float, -1, -1, 0, -1, -1> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<float, -1, 3, 0, -1, 3> >&);
	// generated by autoexplicit.sh
	template void barycenter<Eigen::Matrix<double, -1, 3, 1, -1, 3>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<double, -1, 3, 0, -1, 3> >(Eigen::MatrixBase<Eigen::Matrix<double, -1, 3, 1, -1, 3> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> >&);
	template void barycenter<Eigen::Matrix<double, -1, 3, 1, -1, 3>, Eigen::Matrix<int, -1, 3, 1, -1, 3>, Eigen::Matrix<double, -1, 3, 0, -1, 3> >(Eigen::MatrixBase<Eigen::Matrix<double, -1, 3, 1, -1, 3> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, 3, 1, -1, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> >&);
	template void barycenter<Eigen::Matrix<double, -1, 3, 1, -1, 3>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<double, -1, -1, 0, -1, -1> >(Eigen::MatrixBase<Eigen::Matrix<double, -1, 3, 1, -1, 3> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> >&);
	template void barycenter<Eigen::Matrix<float, -1, 3, 0, -1, 3>, Eigen::Matrix<int, -1, 3, 0, -1, 3>, Eigen::Matrix<float, -1, 3, 0, -1, 3> >(Eigen::MatrixBase<Eigen::Matrix<float, -1, 3, 0, -1, 3> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, 3, 0, -1, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<float, -1, 3, 0, -1, 3> >&);
	template void barycenter<Eigen::Matrix<float, -1, 3, 0, -1, 3>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<float, -1, -1, 0, -1, -1> >(Eigen::MatrixBase<Eigen::Matrix<float, -1, 3, 0, -1, 3> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<float, -1, -1, 0, -1, -1> >&);
	template void barycenter<Eigen::Matrix<float, -1, 3, 1, -1, 3>, Eigen::Matrix<int, -1, 3, 1, -1, 3>, Eigen::Matrix<float, -1, 3, 0, -1, 3> >(Eigen::MatrixBase<Eigen::Matrix<float, -1, 3, 1, -1, 3> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, 3, 1, -1, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<float, -1, 3, 0, -1, 3> >&);
	template void barycenter<Eigen::Matrix<float, -1, 3, 1, -1, 3>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<float, -1, 3, 0, -1, 3> >(Eigen::MatrixBase<Eigen::Matrix<float, -1, 3, 1, -1, 3> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<float, -1, 3, 0, -1, 3> >&);
	template void barycenter<Eigen::Matrix<float, -1, 3, 1, -1, 3>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<float, -1, -1, 0, -1, -1> >(Eigen::MatrixBase<Eigen::Matrix<float, -1, 3, 1, -1, 3> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<float, -1, -1, 0, -1, -1> >&);
	template void barycenter<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<double, -1, 4, 0, -1, 4> >(Eigen::MatrixBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 4, 0, -1, 4> >&);
	template void barycenter<Eigen::Matrix<double, -1, 4, 0, -1, 4>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<double, -1, 4, 0, -1, 4> >(Eigen::MatrixBase<Eigen::Matrix<double, -1, 4, 0, -1, 4> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 4, 0, -1, 4> >&);
	template void barycenter<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<double, -1, 3, 0, -1, 3> >(Eigen::MatrixBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> >&);
	template void barycenter<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<double, -1, -1, 0, -1, -1> >(Eigen::MatrixBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> >&);
	template void barycenter<Eigen::Matrix<double, -1, 3, 0, -1, 3>, Eigen::Matrix<int, -1, 3, 0, -1, 3>, Eigen::Matrix<double, -1, 3, 0, -1, 3> >(Eigen::MatrixBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, 3, 0, -1, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> >&);
	template void barycenter<Eigen::Matrix<double, -1, 3, 0, -1, 3>, Eigen::Matrix<int, -1, 3, 0, -1, 3>, Eigen::Matrix<double, -1, -1, 0, -1, -1> >(Eigen::MatrixBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, 3, 0, -1, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> >&);
	template void barycenter<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, -1, 0, -1, -1>, Eigen::Matrix<double, -1, 2, 0, -1, 2> >(Eigen::MatrixBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, -1, 0, -1, -1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 2, 0, -1, 2> >&);
	template void barycenter<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<double, 2, 3, 0, 2, 3>, Eigen::Matrix<double, 2, 3, 0, 2, 3> >(Eigen::MatrixBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::MatrixBase<Eigen::Matrix<double, 2, 3, 0, 2, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, 2, 3, 0, 2, 3> >&);
	template void barycenter<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<int, 2, 3, 0, 2, 3>, Eigen::Matrix<double, 2, 3, 0, 2, 3> >(Eigen::MatrixBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::MatrixBase<Eigen::Matrix<int, 2, 3, 0, 2, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, 2, 3, 0, 2, 3> >&);
	template void barycenter<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, 1, 0, -1, 1>, Eigen::Matrix<double, -1, 2, 0, -1, 2> >(Eigen::MatrixBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, 1, 0, -1, 1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 2, 0, -1, 2> >&);
	template void barycenter<Eigen::Matrix<double, -1, -1, 0, -1, -1>, Eigen::Matrix<int, -1, 1, 0, -1, 1>, Eigen::Matrix<double, -1, 3, 0, -1, 3> >(Eigen::MatrixBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> > const&, Eigen::MatrixBase<Eigen::Matrix<int, -1, 1, 0, -1, 1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, 3, 0, -1, 3> >&);


	template <
		typename Derivedeye,
		typename Derivedcenter,
		typename Derivedup,
		typename DerivedR>
	void look_at(
		const Eigen::PlainObjectBase<Derivedeye>& eye,
		const Eigen::PlainObjectBase<Derivedcenter>& center,
		const Eigen::PlainObjectBase<Derivedup>& up,
		Eigen::PlainObjectBase<DerivedR>& R)
	{
		typedef Eigen::Matrix<typename DerivedR::Scalar, 3, 1> Vector3S;
		Vector3S f = (center - eye).normalized();
		Vector3S s = f.cross(up).normalized();
		Vector3S u = s.cross(f);
		R = Eigen::Matrix<typename DerivedR::Scalar, 4, 4>::Identity();
		R(0, 0) = s(0);
		R(0, 1) = s(1);
		R(0, 2) = s(2);
		R(1, 0) = u(0);
		R(1, 1) = u(1);
		R(1, 2) = u(2);
		R(2, 0) = -f(0);
		R(2, 1) = -f(1);
		R(2, 2) = -f(2);
		R(0, 3) = -s.transpose() * eye;
		R(1, 3) = -u.transpose() * eye;
		R(2, 3) = f.transpose() * eye;
	}


	template void look_at<Eigen::Matrix<float, 3, 1, 0, 3, 1>, Eigen::Matrix<float, 3, 1, 0, 3, 1>, Eigen::Matrix<float, 3, 1, 0, 3, 1>, Eigen::Matrix<float, 4, 4, 0, 4, 4> >(Eigen::PlainObjectBase<Eigen::Matrix<float, 3, 1, 0, 3, 1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<float, 3, 1, 0, 3, 1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<float, 3, 1, 0, 3, 1> > const&, Eigen::PlainObjectBase<Eigen::Matrix<float, 4, 4, 0, 4, 4> >&);



	template < typename DerivedP>
	void ortho(
		const typename DerivedP::Scalar left,
		const typename DerivedP::Scalar right,
		const typename DerivedP::Scalar bottom,
		const typename DerivedP::Scalar top,
		const typename DerivedP::Scalar nearVal,
		const typename DerivedP::Scalar farVal,
		Eigen::PlainObjectBase<DerivedP>& P)
	{
		P.setIdentity();
		P(0, 0) = 2. / (right - left);
		P(1, 1) = 2. / (top - bottom);
		P(2, 2) = -2. / (farVal - nearVal);
		P(0, 3) = -(right + left) / (right - left);
		P(1, 3) = -(top + bottom) / (top - bottom);
		P(2, 3) = -(farVal + nearVal) / (farVal - nearVal);
	}


	template void ortho<Eigen::Matrix<float, 4, 4, 0, 4, 4> >(Eigen::Matrix<float, 4, 4, 0, 4, 4>::Scalar, Eigen::Matrix<float, 4, 4, 0, 4, 4>::Scalar, Eigen::Matrix<float, 4, 4, 0, 4, 4>::Scalar, Eigen::Matrix<float, 4, 4, 0, 4, 4>::Scalar, Eigen::Matrix<float, 4, 4, 0, 4, 4>::Scalar, Eigen::Matrix<float, 4, 4, 0, 4, 4>::Scalar, Eigen::PlainObjectBase<Eigen::Matrix<float, 4, 4, 0, 4, 4> >&);

	template < typename DerivedP>
	void frustum(
		const typename DerivedP::Scalar left,
		const typename DerivedP::Scalar right,
		const typename DerivedP::Scalar bottom,
		const typename DerivedP::Scalar top,
		const typename DerivedP::Scalar nearVal,
		const typename DerivedP::Scalar farVal,
		Eigen::PlainObjectBase<DerivedP>& P)
	{
		P.setConstant(4, 4, 0.);
		P(0, 0) = (2.0 * nearVal) / (right - left);
		P(1, 1) = (2.0 * nearVal) / (top - bottom);
		P(0, 2) = (right + left) / (right - left);
		P(1, 2) = (top + bottom) / (top - bottom);
		P(2, 2) = -(farVal + nearVal) / (farVal - nearVal);
		P(3, 2) = -1.0;
		P(2, 3) = -(2.0 * farVal * nearVal) / (farVal - nearVal);
	}


	template void frustum<Eigen::Matrix<float, 4, 4, 0, 4, 4> >(Eigen::Matrix<float, 4, 4, 0, 4, 4>::Scalar, Eigen::Matrix<float, 4, 4, 0, 4, 4>::Scalar, Eigen::Matrix<float, 4, 4, 0, 4, 4>::Scalar, Eigen::Matrix<float, 4, 4, 0, 4, 4>::Scalar, Eigen::Matrix<float, 4, 4, 0, 4, 4>::Scalar, Eigen::Matrix<float, 4, 4, 0, 4, 4>::Scalar, Eigen::PlainObjectBase<Eigen::Matrix<float, 4, 4, 0, 4, 4> >&);
	/// Standard value for double epsilon
	const double DOUBLE_EPS = 1.0e-14;
	/// Standard value for double epsilon?
	const double DOUBLE_EPS_SQ = 1.0e-28;
	/// Standard value for single epsilon
	const float FLOAT_EPS = 1.0e-7f;
	/// Standard value for single epsilon?
	const float FLOAT_EPS_SQ = 1.0e-14f;
	template <>  float EPS()
	{
		return FLOAT_EPS;
	}
	template <>  double EPS()
	{
		return DOUBLE_EPS;
	}

	template <>  float EPS_SQ()
	{
		return FLOAT_EPS_SQ;
	}
	template <>  double EPS_SQ()
	{
		return DOUBLE_EPS_SQ;
	}

	template <typename DerivedA, typename DerivedN>
	void null(
		const Eigen::PlainObjectBase<DerivedA>& A,
		Eigen::PlainObjectBase<DerivedN>& N)
	{
		using namespace Eigen;
		typedef typename DerivedA::Scalar Scalar;
		JacobiSVD<Eigen::Matrix<Scalar, Eigen::Dynamic, Eigen::Dynamic> > svd(A, ComputeFullV);
		svd.setThreshold(A.cols() * svd.singularValues().maxCoeff() * EPS<Scalar>());
		N = svd.matrixV().rightCols(A.cols() - svd.rank());
	}
	template void null<Eigen::Matrix<double, 1, 3, 1, 1, 3>, Eigen::Matrix<double, -1, -1, 0, -1, -1> >(Eigen::PlainObjectBase<Eigen::Matrix<double, 1, 3, 1, 1, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<double, -1, -1, 0, -1, -1> >&);
	template void null<Eigen::Matrix<float, 1, 3, 1, 1, 3>, Eigen::Matrix<float, 3, 2, 0, 3, 2> >(Eigen::PlainObjectBase<Eigen::Matrix<float, 1, 3, 1, 1, 3> > const&, Eigen::PlainObjectBase<Eigen::Matrix<float, 3, 2, 0, 3, 2> >&);


	template <typename Qtype>
	void snap_to_fixed_up(
		const Eigen::Quaternion<Qtype>& q,
		Eigen::Quaternion<Qtype>& s)
	{
		using namespace Eigen;
		typedef Eigen::Matrix<Qtype, 3, 1> Vector3Q;
		const Vector3Q up = q.matrix() * Vector3Q(0, 1, 0);
		Vector3Q proj_up(0, up(1), up(2));
		if (proj_up.norm() == 0)
		{
			proj_up = Vector3Q(0, 1, 0);
		}
		proj_up.normalize();
		Quaternion<Qtype> dq;
		dq = Quaternion<Qtype>::FromTwoVectors(up, proj_up);
		s = dq * q;
	}


	// Explicit template instantiations
	template void snap_to_fixed_up<float>(Eigen::Quaternion<float, 0> const&, Eigen::Quaternion<float, 0>&);
	template void snap_to_fixed_up<double>(Eigen::Quaternion<double, 0> const&, Eigen::Quaternion<double, 0>&);



}

template int MOON::bind_vertex_attrib_array<float, -1, -1>(unsigned int, std::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, unsigned int, Eigen::Matrix<float, -1, -1, 1, -1, -1> const&, bool);
// generated by autoexplicit.sh
template int MOON::bind_vertex_attrib_array<float, -1, 3>(unsigned int, std::basic_string<char, std::char_traits<char>, std::allocator<char> > const&, unsigned int, Eigen::Matrix<float, -1, 3, 1, -1, 3> const&, bool);



