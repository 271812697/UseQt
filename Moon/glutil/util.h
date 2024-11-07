#pragma once
#include <Eigen/Core>
#include <string>
#include <map>
namespace MOON
{

	bool create_shader_program(
		const std::string& geom_source,
		const std::string& vert_source,
		const std::string& frag_source,
		const std::map<std::string, unsigned int>& attrib,
		unsigned int& id);
	bool create_shader_program(
		const std::string& vert_source,
		const std::string& frag_source,
		const std::map<std::string, unsigned int>& attrib,
		unsigned int& id);

	unsigned int create_shader_program(
		const std::string& geom_source,
		const std::string& vert_source,
		const std::string& frag_source,
		const std::map<std::string, unsigned int>& attrib);

	unsigned int create_shader_program(
		const std::string& vert_source,
		const std::string& frag_source,
		const std::map<std::string, unsigned int>& attrib);

	template <typename Scalar, int RowsAtCompileTime, int ColsAtCompileTime>
	int bind_vertex_attrib_array(
		const unsigned int program_shader,
		const std::string& name,
		unsigned int bufferID,
		const Eigen::Matrix<Scalar, RowsAtCompileTime, ColsAtCompileTime, Eigen::RowMajor>& M,
		const bool refresh);


	template <
		typename DerivedV,
		typename DerivedF,
		typename DerivedBC>
	void barycenter(
		const Eigen::MatrixBase<DerivedV>& V,
		const Eigen::MatrixBase<DerivedF>& F,
		Eigen::PlainObjectBase<DerivedBC>& BC);
	template <
		typename Derivedeye,
		typename Derivedcenter,
		typename Derivedup,
		typename DerivedR >
	void look_at(
		const Eigen::PlainObjectBase<Derivedeye>& eye,
		const Eigen::PlainObjectBase<Derivedcenter>& center,
		const Eigen::PlainObjectBase<Derivedup>& up,
		Eigen::PlainObjectBase<DerivedR>& R);

	template < typename DerivedP>
	void ortho(
		const typename DerivedP::Scalar left,
		const typename DerivedP::Scalar right,
		const typename DerivedP::Scalar bottom,
		const typename DerivedP::Scalar top,
		const typename DerivedP::Scalar nearVal,
		const typename DerivedP::Scalar farVal,
		Eigen::PlainObjectBase<DerivedP>& P);

	template < typename DerivedP>
	void frustum(
		const typename DerivedP::Scalar left,
		const typename DerivedP::Scalar right,
		const typename DerivedP::Scalar bottom,
		const typename DerivedP::Scalar top,
		const typename DerivedP::Scalar nearVal,
		const typename DerivedP::Scalar farVal,
		Eigen::PlainObjectBase<DerivedP>& P);

	template <typename DerivedA, typename DerivedN>
	void null(
		const Eigen::PlainObjectBase<DerivedA>& A,
		Eigen::PlainObjectBase<DerivedN>& N);

	/// Function returning EPS for corresponding type
	template <typename S_type>  S_type EPS();
	/// Function returning EPS_SQ for corresponding type
	template <typename S_type>  S_type EPS_SQ();
	// Template specializations for float and double
	template <>  float EPS<float>();
	template <>  double EPS<double>();
	template <>  float EPS_SQ<float>();
	template <>  double EPS_SQ<double>();



	template <typename Qtype>
	void snap_to_fixed_up(
		const Eigen::Quaternion<Qtype>& q,
		Eigen::Quaternion<Qtype>& s);



}

