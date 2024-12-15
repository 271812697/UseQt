#pragma once
#include <Eigen/Core>
#include <cstdint>

namespace MOON
{


	struct  VertexData
	{
		Eigen::Vector4<float>  m_positionSize; // xyz = position, w = size
		Eigen::Vector4<uint8_t>  m_color;        // rgba8 (MSB = r)

		VertexData() {}
		VertexData(const Eigen::Vector3<float>& _position, float _size, Eigen::Vector4<uint8_t> _color) : m_positionSize(_position.x(), _position.y(), _position.z(), _size), m_color(_color) {}
	};

	struct CameraParam {
		float viewportWidth;
		float viewportHeight;
		float projectY;
		bool orthProj = false;
		Eigen::Vector3<float> eye;
		Eigen::Vector3<float>viewDirectioin;
		Eigen::Vector3<float>rayOrigin;
		Eigen::Vector3<float>rayDirection;
		Eigen::Matrix4f view;
		Eigen::Matrix4f proj;
		Eigen::Matrix4f viewProj;

	};
	class Guizmo
	{
	public:

		Guizmo();
		void Draw(Eigen::Matrix4f& model);
		void DrawQuadFilled(const Eigen::Vector3<float>& _origin, const Eigen::Vector3<float>& _normal, const Eigen::Vector2<float>& _size);
		void DrawTeapot(const Eigen::Matrix4f& model, const Eigen::Matrix4f& viewProj);
		void DrawQuadFilled(const Eigen::Vector3<float>& _a, const Eigen::Vector3<float>& _b, const Eigen::Vector3<float>& _c, const Eigen::Vector3<float>& _d);
		void DrawArrow(const Eigen::Vector3<float>& _start, const Eigen::Vector3<float>& _end, float _headLength = -1.0f, float _headThickness = -1.0f);
		CameraParam& getCameraParam();
		void Submit();
		void Clear();
		~Guizmo();
		float  pixelsToWorldSize(const Eigen::Vector3<float>& _position, float _pixels);
	private:
		unsigned int lineShader;
		unsigned int pointShader;
		unsigned int triangleShader;
		unsigned int teapotShader;
		std::vector<VertexData> lineVertexData;
		std::vector<VertexData> pointVertexData;
		std::vector<VertexData> triangleVertexData;
		std::vector<Eigen::Vector4<uint8_t>> colorStack;
		std::vector<float> alphaStack;
		std::vector<float> sizeStack;

		std::vector<Eigen::Matrix4f> matrixStack;
		CameraParam cameraParam;


	};
}

