#pragma once
#include <Eigen/Core>
#include <cstdint>
namespace MOON
{
	class GridRenderer
	{
	public:
		typedef unsigned int GLuint;
		typedef unsigned int GLint;
		bool is_initialized = false;
		GLuint grid_vao;
		GLuint grid_shader;
		GridRenderer();
		void DrawGrid(Eigen::Vector3<float> cam_pos, Eigen::Matrix4f viewproj);
		~GridRenderer();
	};
}

