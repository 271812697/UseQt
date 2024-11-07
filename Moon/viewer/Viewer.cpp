#include "Viewer.h"
#include<glad/glad.h>
#include "report_gl_error.h"
#include"project.h"
#include"unproject.h"
#include"two_axis_valuator_fixed_up.h"
#include "snap_to_canonical_view_quat.h"

namespace Geomerty
{

	bool Viewer::mouse_down(MouseButton button)
	{
		// Remember mouse location at down even if used by callback/plugin
		down_mouse_x = current_mouse_x;
		down_mouse_y = current_mouse_y;
		down = true;
		down_translation = core().camera_translation;
		// Initialization code for the trackball
		Eigen::RowVector3d center = Eigen::RowVector3d(0, 0, 0);
		if (data().V.rows() > 0)
		{
			// be careful that V may be 2D
			center.head(data().V.cols()) = data().V.colwise().sum() / data().V.rows();
		}
		Eigen::Vector3f coord =
			project(
				Eigen::Vector3f(center(0), center(1), center(2)),
				core().view,
				core().proj,
				core().viewport);
		down_mouse_z = coord[2];
		down_rotation = core().trackball_angle;
		mouse_mode = MouseMode::Rotation;
		switch (button)
		{
		case MouseButton::Left:
			if (core().rotation_type == ViewerCore::ROTATION_TYPE_NO_ROTATION) {
				mouse_mode = MouseMode::Translation;
			}
			else {
				mouse_mode = MouseMode::Rotation;
			}
			break;
		case MouseButton::Right:
			mouse_mode = MouseMode::Translation;
			break;
		default:
			mouse_mode = MouseMode::None;
			break;
		}
		return true;
	}
	bool Viewer::mouse_up(MouseButton button)
	{
		down = false;
		mouse_mode = MouseMode::None;
		return true;
	}
	bool Viewer::mouse_move(int mouse_x, int mouse_y)
	{
		if (hack_never_moved)
		{
			down_mouse_x = mouse_x;
			down_mouse_y = mouse_y;
			hack_never_moved = false;
		}
		current_mouse_x = mouse_x;
		current_mouse_y = mouse_y;
		if (down)
		{

			switch (mouse_mode)
			{
			case MouseMode::Rotation:
			{
				two_axis_valuator_fixed_up(
					core().viewport(2), core().viewport(3),
					2.0,
					down_rotation,
					down_mouse_x - core().viewport(0),
					down_mouse_y - (height - core().viewport(1) - core().viewport(3)),
					mouse_x - core().viewport(0),
					mouse_y - (height - core().viewport(1) - core().viewport(3)),
					core().trackball_angle);
				break;
			}

			case MouseMode::Translation:
			{
				//translation
				Eigen::Vector3f pos1 = unproject(Eigen::Vector3f(mouse_x, core().viewport[3] - mouse_y, down_mouse_z), core().view, core().proj, core().viewport);
				Eigen::Vector3f pos0 = unproject(Eigen::Vector3f(down_mouse_x, core().viewport[3] - down_mouse_y, down_mouse_z), core().view, core().proj, core().viewport);

				Eigen::Vector3f diff = pos1 - pos0;
				core().camera_translation = down_translation + Eigen::Vector3f(diff[0], diff[1], diff[2]);

				break;
			}
			case MouseMode::Zoom:
			{
				float delta = 0.001f * (mouse_x - down_mouse_x + mouse_y - down_mouse_y);
				core().camera_zoom *= 1 + delta;
				down_mouse_x = mouse_x;
				down_mouse_y = mouse_y;
				break;
			}

			default:
				break;
			}
		}
		return true;
	}
	bool Viewer::mouse_scroll(float delta_y)
	{
		// Only zoom if there's actually a change
		if (delta_y != 0)
		{
			float mult = (1.0 + ((delta_y > 0) ? 1. : -1.) * 0.05);
			const float min_zoom = 0.1f;
			core().camera_zoom = core().camera_zoom * mult;// (core().camera_zoom * mult > min_zoom ? core().camera_zoom * mult : min_zoom);
		}
		return true;
	}
	void Viewer::init()
	{
		for (auto& core : this->core_list)
		{
			core.init();
		}


	}
	Viewer::Viewer() :
		data_list(1),
		selected_data_index(0),
		next_data_id(1),
		selected_core_index(0),
		next_core_id(2)
	{
		data_list.front().id = 0;
		core_list.emplace_back(ViewerCore());
		core_list.front().id = 1;
		// Per face
		data().set_face_based(false);
	}

	Viewer::~Viewer()
	{
	}
	void Viewer::draw()
	{
		using namespace std;
		using namespace Eigen;
		for (auto& core : core_list)
		{
			core.clear_framebuffers();
		}
		// Shadow pass
		for (auto& core : core_list)
		{
			if (core.is_shadow_mapping)
			{
				core.initialize_shadow_pass();
				for (auto& mesh : data_list)
				{
					if (mesh.is_visible & core.id)
					{
						core.draw_shadow_pass(mesh);
					}
				}
				core.deinitialize_shadow_pass();
			}
		}
		for (auto& core : core_list)
		{
			for (auto& mesh : data_list)
			{
				if (mesh.is_visible & core.id)
				{
					core.draw(mesh);
				}
			}
		}

	}

	template <typename T>
	void Viewer::draw_buffer(
		Geomerty::ViewerCore& core,
		Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>& R,
		Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>& G,
		Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>& B,
		Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>& A,
		Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>& D)
	{
		// follows Geomerty::ViewerCore::draw_buffer, image is transposed from
		// typical matrix view
		const int width = R.rows() ? R.rows() : core.viewport(2);
		const int height = R.cols() ? R.cols() : core.viewport(3);
		R.resize(width, height);
		G.resize(width, height);
		B.resize(width, height);
		A.resize(width, height);
		D.resize(width, height);

		////////////////////////////////////////////////////////////////////////
		// Create an initial multisampled framebuffer
		////////////////////////////////////////////////////////////////////////
		unsigned int framebuffer;
		unsigned int color_buffer;
		unsigned int depth_buffer;
		{
			glGenFramebuffers(1, &framebuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			// create a multisampled color attachment texture (is a texture really
			// needed? Could this be a renderbuffer instead?)
			glGenTextures(1, &color_buffer);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, color_buffer);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA, width, height, GL_TRUE);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, color_buffer, 0);
			// create a (also multisampled) renderbuffer object for depth and stencil attachments
			glGenRenderbuffers(1, &depth_buffer);
			glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
			glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, width, height);
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depth_buffer);


			assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
			report_gl_error("glCheckFramebufferStatus: ");
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		////////////////////////////////////////////////////////////////////////
		// configure second post-processing framebuffer
		////////////////////////////////////////////////////////////////////////
		unsigned int intermediateFBO;
		unsigned int screenTexture, depthTexture;
		{
			glGenFramebuffers(1, &intermediateFBO);
			glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);
			// create a color attachment texture
			glGenTextures(1, &screenTexture);
			glBindTexture(GL_TEXTURE_2D, screenTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);

			// create depth attachment texture
			glGenTextures(1, &depthTexture);
			glBindTexture(GL_TEXTURE_2D, depthTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

			assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		////////////////////////////////////////////////////////////////////////
		// attach initial framebuffer and draw all `data`
		////////////////////////////////////////////////////////////////////////
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		// Clear the buffer
		glClearColor(
			core.background_color(0),
			core.background_color(1),
			core.background_color(2),
			core.background_color(3));
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Save old viewport
		Eigen::Vector4f viewport_ori = core.viewport;
		core.viewport << 0, 0, width, height;
		// Draw all `data`
		for (auto& data : data_list)
		{
			if (data.is_visible & core.id)
			{
				core.draw(data);
			}
		}
		// Restore viewport
		core.viewport = viewport_ori;

		////////////////////////////////////////////////////////////////////////
		// attach second framebuffer and redraw (for anti-aliasing?)
		////////////////////////////////////////////////////////////////////////
		glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
		report_gl_error("before: ");
		glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
		report_gl_error("glBlitFramebuffer: ");


		////////////////////////////////////////////////////////////////////////
		// Read pixel data from framebuffer, write into buffers
		////////////////////////////////////////////////////////////////////////
		glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);
		// Copy back in the given Eigen matrices
		{
			typedef typename std::conditional< std::is_floating_point<T>::value, GLfloat, GLubyte>::type GLType;
			GLenum type = std::is_floating_point<T>::value ? GL_FLOAT : GL_UNSIGNED_BYTE;
			GLType* pixels = (GLType*)calloc(width * height * 4, sizeof(GLType));
			GLType* depth = (GLType*)calloc(width * height * 1, sizeof(GLType));
			glReadPixels(0, 0, width, height, GL_RGBA, type, pixels);
			glReadPixels(0, 0, width, height, GL_DEPTH_COMPONENT, type, depth);
			int count = 0;
			for (unsigned j = 0; j < height; ++j)
			{
				for (unsigned i = 0; i < width; ++i)
				{
					R(i, j) = pixels[count * 4 + 0];
					G(i, j) = pixels[count * 4 + 1];
					B(i, j) = pixels[count * 4 + 2];
					A(i, j) = pixels[count * 4 + 3];
					D(i, j) = depth[count * 1 + 0];
					++count;
				}
			}
			// Clean up
			free(pixels);
			free(depth);
		}

		// Clean up
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDeleteTextures(1, &screenTexture);
		glDeleteTextures(1, &depthTexture);
		glDeleteTextures(1, &color_buffer);
		glDeleteRenderbuffers(1, &depth_buffer);
		glDeleteFramebuffers(1, &framebuffer);
		glDeleteFramebuffers(1, &intermediateFBO);
	}

	void Viewer::resize(int w, int h)
	{

		post_resize(w, h);
	}

	void Viewer::post_resize(int w, int h)
	{
		width = w;
		height = h;
		if (core_list.size() == 1)
		{
			core().viewport = Eigen::Vector4f(0, 0, w, h);
		}
		else
		{
			// It is up to the user to define the behavior of the post_resize() function
			// when there are multiple viewports (through the `callback_post_resize` callback)
		}
	}

	void Viewer::snap_to_canonical_quaternion()
	{
		Eigen::Quaternionf snapq = this->core().trackball_angle;
		snap_to_canonical_view_quat(snapq, 1.0f, this->core().trackball_angle);
	}


	ViewerData& Viewer::data(int mesh_id /*= -1*/)
	{
		assert(!data_list.empty() && "data_list should never be empty");
		int index;
		if (mesh_id == -1)
			index = selected_data_index;
		else
			index = mesh_index(mesh_id);

		assert((index >= 0 && index < data_list.size()) &&
			"selected_data_index or mesh_id should be in bounds");
		return data_list[index];
	}

	const ViewerData& Viewer::data(int mesh_id /*= -1*/) const
	{
		assert(!data_list.empty() && "data_list should never be empty");
		int index;
		if (mesh_id == -1)
			index = selected_data_index;
		else
			index = mesh_index(mesh_id);

		assert((index >= 0 && index < data_list.size()) &&
			"selected_data_index or mesh_id should be in bounds");
		return data_list[index];
	}

	int Viewer::append_mesh(bool visible /*= true*/)
	{
		assert(data_list.size() >= 1);

		data_list.emplace_back();
		selected_data_index = data_list.size() - 1;
		data_list.back().id = next_data_id++;
		if (visible)
			for (int i = 0; i < core_list.size(); i++)
				data_list.back().set_visible(true, core_list[i].id);
		else
			data_list.back().is_visible = 0;
		return data_list.back().id;
	}

	bool Viewer::erase_mesh(const size_t index)
	{
		assert((index >= 0 && index < data_list.size()) && "index should be in bounds");
		assert(data_list.size() >= 1);
		if (data_list.size() == 1)
		{
			// Cannot remove last mesh
			return false;
		}
		data_list[index].meshgl.free();
		data_list.erase(data_list.begin() + index);
		if (selected_data_index >= index && selected_data_index > 0)
		{
			selected_data_index--;
		}
		return true;
	}
	size_t Viewer::mesh_index(const int id) const {
		for (size_t i = 0; i < data_list.size(); ++i)
		{
			if (data_list[i].id == id)
				return i;
		}
		return 0;
	}
	ViewerCore& Viewer::core(unsigned core_id /*= 0*/)
	{
		assert(!core_list.empty() && "core_list should never be empty");
		int core_index;
		if (core_id == 0)
			core_index = selected_core_index;
		else
			core_index = this->core_index(core_id);
		assert((core_index >= 0 && core_index < core_list.size()) && "selected_core_index should be in bounds");
		return core_list[core_index];
	}
	const ViewerCore& Viewer::core(unsigned core_id /*= 0*/) const
	{
		assert(!core_list.empty() && "core_list should never be empty");
		int core_index;
		if (core_id == 0)
			core_index = selected_core_index;
		else
			core_index = this->core_index(core_id);
		assert((core_index >= 0 && core_index < core_list.size()) && "selected_core_index should be in bounds");
		return core_list[core_index];
	}

	bool Viewer::erase_core(const size_t index)
	{
		assert((index >= 0 && index < core_list.size()) && "index should be in bounds");
		assert(data_list.size() >= 1);
		if (core_list.size() == 1)
		{
			// Cannot remove last viewport
			return false;
		}
		core_list[index].shut(); // does nothing
		core_list.erase(core_list.begin() + index);
		if (selected_core_index >= index && selected_core_index > 0)
		{
			selected_core_index--;
		}
		return true;
	}
	size_t Viewer::core_index(const int id) const {
		for (size_t i = 0; i < core_list.size(); ++i)
		{
			if (core_list[i].id == id)
				return i;
		}
		return 0;
	}
	int Viewer::append_core(Eigen::Vector4f viewport, bool append_empty /*= false*/)
	{
		core_list.push_back(core()); // copies the previous active core and only changes the viewport
		core_list.back().viewport = viewport;
		core_list.back().id = next_core_id;
		next_core_id <<= 1;
		if (!append_empty)
		{
			for (auto& data : data_list)
			{
				data.set_visible(true, core_list.back().id);
				data.copy_options(core(), core_list.back());
			}
		}
		selected_core_index = core_list.size() - 1;
		return core_list.back().id;
	}
} // end namespace

template void Geomerty::Viewer::draw_buffer<unsigned char>(Geomerty::ViewerCore&, Eigen::Matrix<unsigned char, -1, -1, 0, -1, -1>&, Eigen::Matrix<unsigned char, -1, -1, 0, -1, -1>&, Eigen::Matrix<unsigned char, -1, -1, 0, -1, -1>&, Eigen::Matrix<unsigned char, -1, -1, 0, -1, -1>&, Eigen::Matrix<unsigned char, -1, -1, 0, -1, -1>&);
template void Geomerty::Viewer::draw_buffer<double>(Geomerty::ViewerCore&, Eigen::Matrix<double, -1, -1, 0, -1, -1>&, Eigen::Matrix<double, -1, -1, 0, -1, -1>&, Eigen::Matrix<double, -1, -1, 0, -1, -1>&, Eigen::Matrix<double, -1, -1, 0, -1, -1>&, Eigen::Matrix<double, -1, -1, 0, -1, -1>&);
template void Geomerty::Viewer::draw_buffer<float>(Geomerty::ViewerCore&, Eigen::Matrix<float, -1, -1, 0, -1, -1>&, Eigen::Matrix<float, -1, -1, 0, -1, -1>&, Eigen::Matrix<float, -1, -1, 0, -1, -1>&, Eigen::Matrix<float, -1, -1, 0, -1, -1>&, Eigen::Matrix<float, -1, -1, 0, -1, -1>&);

