#pragma once
#include "MeshGL.h"

#include <Eigen/Geometry>
#include <Eigen/Core>


namespace Geomerty
{

	// Forward declaration
	class ViewerData;

	/// Basic class of the 3D mesh viewer
	class ViewerCore
	{
	public:
		using GLuint = MeshGL::GLuint;
		ViewerCore();

		/// Initialization
		void init();

		/// Shutdown
		void shut();

		/// Serialization code
		void InitSerialization();

		/// Adjust the view to see the entire model
		/// @param[in] V  #V by 3 list of vertex positions
		/// @param[in] F  #F by 3 list of triangle indices into V
		void align_camera_center(
			const Eigen::MatrixXd& V,
			const Eigen::MatrixXi& F);
		/// \overload
		void align_camera_center(
			const Eigen::MatrixXd& V);

		/// Determines how much to zoom and shift such that the mesh fills the unit
		/// box (centered at the origin)
		/// @param[in] V  #V by 3 list of vertex positions
		/// @param[in] F  #F by 3 list of triangle indices into V
		/// @param[out] zoom  zoom factor
		/// @param[out] shift  3d shift
		void get_scale_and_shift_to_fit_mesh(
			const Eigen::MatrixXd& V,
			const Eigen::MatrixXi& F,
			float& zoom,
			Eigen::Vector3f& shift);
		/// \overload
		void get_scale_and_shift_to_fit_mesh(
			const Eigen::MatrixXd& V,
			float& zoom,
			Eigen::Vector3f& shift);

		/// Clear the frame buffers
		void clear_framebuffers();

		/// Draw everything
		///
		/// \note data cannot be const because it is being set to "clean"
		///
		/// @param[in] data  which ViewerData to draw
		/// @param[in] update_matrices  whether to update view, proj, and norm
		///   matrices in shaders
		void draw(ViewerData& data, bool update_matrices = true);
		/// initialize shadow pass
		void initialize_shadow_pass();
		/// deinitialize shadow pass
		void deinitialize_shadow_pass();
		/// Draw everything to shadow map
		/// @param[in] data  which ViewerData to draw
		/// @param[in] update_matrices  whether to update view, proj, and norm
		void draw_shadow_pass(ViewerData& data, bool update_matrices = true);
		/// Render given ViewerData to a buffer. The width and height are determined by
		/// non-zeros dimensions of R (and G,B,A should match) or – if both are zero —
		/// are set to this core's viewport sizes.
		///
		/// @param[in] data  which ViewerData to draw
		/// @param[in] update_matrices  whether to update view, proj, and norm matrices in
		///     shaders
		/// @param[out] R  width by height red pixel color values
		/// @param[out] G  width by height green pixel color values
		/// @param[out] B  width by height blue pixel color values
		/// @param[out] A  width by height alpha pixel color values
		///
		void draw_buffer(
			ViewerData& data,
			bool update_matrices,
			Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic>& R,
			Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic>& G,
			Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic>& B,
			Eigen::Matrix<unsigned char, Eigen::Dynamic, Eigen::Dynamic>& A);
		/// Draw the text lables
		/// @param[in] data  which ViewerData to draw
		/// @param[in] labels  text labels to draw
		void draw_labels(
			ViewerData& data,
			const Geomerty::MeshGL::TextGL& labels
		);

		/// Type of user interface for changing the view rotation based on the mouse
		/// draggin.
		enum RotationType
		{
			/// Typical trackball rotation (like Meshlab)
			ROTATION_TYPE_TRACKBALL = 0,
			/// Fixed up rotation (like Blender, Maya, etc.)
			ROTATION_TYPE_TWO_AXIS_VALUATOR_FIXED_UP = 1,
			/// No rotation suitable for 2D 
			ROTATION_TYPE_NO_ROTATION = 2,
			/// Total number of rotation types
			NUM_ROTATION_TYPES = 3
		};
		/// Set the current rotation type
		/// @param[in] value  the new rotation type
		void set_rotation_type(const RotationType& value);

		/// Set a ViewerData visualization option for this viewport
		/// @param[in] property_mask  a bit mask of visualization option
		/// @param[in] value  whether to set or unset the property
		void set(unsigned int& property_mask, bool value = true) const;

		/// Unset a ViewerData visualization option for this viewport
		/// @param[in] property_mask  a bit mask of visualization option
		void unset(unsigned int& property_mask) const;

		/// Toggle a ViewerData visualization option for this viewport
		/// @param[in] property_mask  a bit mask of visualization option
		void toggle(unsigned int& property_mask) const;

		/// Check whether a ViewerData visualization option is set for this viewport
		/// @param[in] property_mask  a bit mask of visualization option
		/// @returns whether the property is set
		bool is_set(unsigned int property_mask) const;

		/// delete the shadow buffers
		void delete_shadow_buffers();
		/// generate the shadow buffers
		void generate_shadow_buffers();

		// ------------------- Properties

		/// Unique identifier
		unsigned int id = 1u;

		/// Background color as RGBA
		Eigen::Vector4f background_color;

		/// Light position (or direction to light)
		Eigen::Vector3f light_position;
		/// Whether to treat `light_position` as a point or direction
		bool is_directional_light;
		/// Whether shadow mapping is on
		bool is_shadow_mapping;
		/// Width of the shadow map
		GLuint shadow_width;
		/// Height of the shadow map
		GLuint shadow_height;
		/// Shadow map depth texture
		GLuint shadow_depth_tex;
		/// Shadow map depth framebuffer object
		GLuint shadow_depth_fbo;
		/// Shadow map color render buffer object
		GLuint shadow_color_rbo;
		/// Factor of lighting (0: no lighting, 1: full lighting)
		float lighting_factor;

		/// Type of rotation interaction
		RotationType rotation_type;
		/// View rotation as quaternion
		Eigen::Quaternionf trackball_angle;

		/// Base zoom of camera
		float camera_base_zoom;
		/// Current zoom of camera
		float camera_zoom;
		/// Whether camera is orthographic (or perspective)
		bool orthographic;
		/// Base translation of camera
		Eigen::Vector3f camera_base_translation;
		/// Current translation of camera
		Eigen::Vector3f camera_translation;
		/// Current "eye" / origin position of camera
		Eigen::Vector3f camera_eye;
		/// Current "up" vector of camera
		Eigen::Vector3f camera_up;
		/// Current "look at" position of camera
		Eigen::Vector3f camera_center;
		/// Current view angle of camera
		float camera_view_angle;
		/// Near plane of camera
		float camera_dnear;
		/// Far plane of camera
		float camera_dfar;

		/// Whether testing for depth is enabled
		bool depth_test;

		/// Whether "animating" (continuous drawing) is enabled
		bool is_animating;
		/// Max fps of animation loop (e.g. 30fps or 60fps)
		double animation_max_fps;

		/// Caches the two-norm between the min/max point of the bounding box
		float object_scale;

		/// Viewport size
		Eigen::Vector4f viewport;

		/// OpenGL view transformation matrix on last render pass
		Eigen::Matrix4f view;
		/// OpenGL proj transformation matrix on last render pass
		Eigen::Matrix4f proj;
		/// OpenGL norm transformation matrix on last render pass
		Eigen::Matrix4f norm;
		/// OpenGL shadow_view transformation matrix on last render pass
		Eigen::Matrix4f shadow_view;
		/// OpenGL shadow_proj transformation matrix on last render pass
		Eigen::Matrix4f shadow_proj;
	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	};

}

