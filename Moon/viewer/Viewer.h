#pragma once
#include "MeshGL.h"
#include "ViewerCore.h"
#include "ViewerData.h"
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <vector>
#include <string>
#include <cstdint>

namespace MOON
{
	class Viewer
	{
	public:
		// UI Enumerations
		enum class MouseButton { Left, Middle, Right };
		enum class MouseMode { None, Rotation, Zoom, Pan, Translation } mouse_mode;
		Eigen::Quaternionf down_rotation;
		int current_mouse_x;
		int current_mouse_y;
		int down_mouse_x;
		int down_mouse_y;
		float down_mouse_z;
		Eigen::Vector3f down_translation;
		bool down;
		bool hack_never_moved = true;
		int width = 1;
		int height = 1;
		bool mouse_down(MouseButton button);
		bool mouse_up(MouseButton button);
		bool mouse_move(int mouse_x, int mouse_y);
		bool mouse_scroll(float delta_y);
		void init();
		Viewer();
		~Viewer();
		// Mesh IO
		// Draw everything
		void draw();
		template <typename T>
		void draw_buffer(
			// can't be const because of writing in and out of `core.viewport`
			/*const*/ MOON::ViewerCore& core,
			Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>& R,
			Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>& G,
			Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>& B,
			Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>& A,
			Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>& D);

		void resize(int w, int h); // explicitly set window size
		void post_resize(int w, int h); // external resize due to user interaction

		//void snap_to_canonical_quaternion();

		ViewerData& data(int mesh_id = -1);
		const ViewerData& data(int mesh_id = -1) const;

		int append_mesh(bool visible = true);
		bool erase_mesh(const size_t index);

		size_t mesh_index(const int id) const;

		ViewerCore& core(unsigned core_id = 0);
		const ViewerCore& core(unsigned core_id = 0) const;

		int append_core(Eigen::Vector4f viewport, bool append_empty = false);

		bool erase_core(const size_t index);

		size_t core_index(const int id) const;
	public:

		std::vector<ViewerData> data_list;
		size_t selected_data_index;
		int next_data_id;
		// Stores all the viewing options
		std::vector<ViewerCore> core_list;
		size_t selected_core_index;
		int next_core_id;
	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	};

} // end namespace

