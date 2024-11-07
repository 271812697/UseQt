#pragma once
#include "MeshGL.h"
#include <cassert>
#include <cstdint>
#include <Eigen/Core>
#include <memory>
#include <vector>
namespace MOON
{
	class ViewerCore;
	class ViewerData
	{
	public:
		ViewerData();
		/// Empty all fields
		void clear();
		void set_face_based(bool newvalue);
		void set_mesh(const Eigen::MatrixXd& V, const Eigen::MatrixXi& F);

		void set_vertices(const Eigen::MatrixXd& V);

		void set_normals(const Eigen::MatrixXd& N);

		void set_visible(bool value, unsigned int core_id = 1);

		void set_colors(const Eigen::MatrixXd& C);

		void set_uv(const Eigen::MatrixXd& UV);

		void set_uv(const Eigen::MatrixXd& UV_V, const Eigen::MatrixXi& UV_F);

		void set_texture(
			const Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic>& R,
			const Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic>& G,
			const Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic>& B,
			const Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic>& A);
		/// \overload
		void set_texture(
			const Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic>& R,
			const Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic>& G,
			const Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic>& B);



		void set_colormap(const Eigen::MatrixXd& CM);

		void set_points(
			const Eigen::MatrixXd& P,
			const Eigen::MatrixXd& C);

		void add_points(const Eigen::MatrixXd& P, const Eigen::MatrixXd& C);

		/// Clear the point data
		void clear_points();

		void set_edges(const Eigen::MatrixXd& P, const Eigen::MatrixXi& E, const Eigen::MatrixXd& C);
		void add_edges(const Eigen::MatrixXd& P1, const Eigen::MatrixXd& P2, const Eigen::MatrixXd& C);

		void set_edges_from_vector_field(
			const Eigen::MatrixXd& P,
			const Eigen::MatrixXd& V,
			const Eigen::MatrixXd& C);

		void clear_edges();

		void set_labels(const Eigen::MatrixXd& P, const std::vector<std::string>& str);

		void add_label(const Eigen::VectorXd& P, const std::string& str);

		/// Clear the label data
		void clear_labels();

		/// Computes the normals of the mesh
		void compute_normals();


		void uniform_colors(
			const Eigen::Vector3d& ambient,
			const Eigen::Vector3d& diffuse,
			const Eigen::Vector3d& specular);
		/// \overload
		void uniform_colors(
			const Eigen::Vector4d& ambient,
			const Eigen::Vector4d& diffuse,
			const Eigen::Vector4d& specular);

		/// Generate a normal image matcap
		void normal_matcap();

		/// Generates a default grid texture (without uvs)
		void grid_texture();

		void copy_options(const ViewerCore& from, const ViewerCore& to);

		/// Vertices of the current mesh (#V x 3)
		Eigen::MatrixXd V;
		/// Faces of the mesh (#F x 3)
		Eigen::MatrixXi F;

		// Per face attributes
		/// One normal per face
		Eigen::MatrixXd F_normals;

		/// Per face ambient color
		Eigen::MatrixXd F_material_ambient;
		/// Per face diffuse color
		Eigen::MatrixXd F_material_diffuse;
		/// Per face specular color
		Eigen::MatrixXd F_material_specular;

		// Per vertex attributes
		/// One normal per vertex
		Eigen::MatrixXd V_normals;

		/// Per vertex ambient color
		Eigen::MatrixXd V_material_ambient;
		/// Per vertex diffuse color
		Eigen::MatrixXd V_material_diffuse;
		/// Per vertex specular color
		Eigen::MatrixXd V_material_specular;
		// UV parametrization
		/// UV vertices
		Eigen::MatrixXd V_uv;
		/// optional faces for UVs
		Eigen::MatrixXi F_uv;
		// Texture
		/// Texture red colors
		Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> texture_R;
		/// Texture green colors
		Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> texture_G;
		/// Texture blue colors
		Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> texture_B;
		/// Texture alpha values
		Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic> texture_A;
		// Overlays
		Eigen::MatrixXd lines;
		Eigen::MatrixXd points;
		Eigen::MatrixXd vertex_labels_positions;
		/// Text positions of faces
		Eigen::MatrixXd face_labels_positions;
		/// Text positions of labels
		Eigen::MatrixXd labels_positions;
		/// Text strings of labels at vertices
		std::vector<std::string> vertex_labels_strings;
		/// Text strings of labels at faces
		std::vector<std::string> face_labels_strings;
		/// Text strings of labels
		std::vector<std::string> labels_strings;
		/// Marks dirty buffers that need to be uploaded to OpenGL
		uint16_t dirty;
		/// Enable per-face or per-vertex properties
		bool face_based;

		/// Enable double-sided lighting on faces
		bool double_sided;

		/// Invert mesh normals
		bool invert_normals;

		unsigned int is_visible;
		unsigned int show_custom_labels;
		unsigned int show_face_labels;
		unsigned int show_faces;
		unsigned int show_lines;
		unsigned int show_overlay;
		unsigned int show_overlay_depth;
		unsigned int show_texture;
		unsigned int show_vertex_labels;
		unsigned int use_matcap;
		float point_size;
		float line_width;
		float label_size;
		Eigen::Matrix<float, 4, 1, Eigen::DontAlign> line_color;
		Eigen::Matrix<float, 4, 1, Eigen::DontAlign> label_color;
		float shininess;
		int id;
		MOON::MeshGL meshgl;
		void update_labels(
			MOON::MeshGL::TextGL& GL_labels,
			const Eigen::MatrixXd& positions,
			const std::vector<std::string>& strings
		);
		void updateGL(
			const MOON::ViewerData& data,
			const bool invert_normals,
			MOON::MeshGL& meshgl);
	};

} // 
