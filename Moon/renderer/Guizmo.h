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
	typedef std::vector<VertexData> VertexList;
	enum DrawPrimitiveType
	{
		// order here determines the order in which unsorted primitives are drawn
		DrawPrimitive_Triangles,
		DrawPrimitive_Lines,
		DrawPrimitive_Points,

		DrawPrimitive_Count
	};

	struct DrawList
	{
		unsigned int m_layerId;
		DrawPrimitiveType m_primType;
		const VertexData* m_vertexData;
		unsigned int m_vertexCount;
	};

	enum Key
	{
		Mouse_Left,
		Key_L,
		Key_R,
		Key_S,
		Key_T,

		Key_Count,


		Action_Select = Mouse_Left,
		Action_GizmoLocal = Key_L,
		Action_GizmoRotation = Key_R,
		Action_GizmoScale = Key_S,
		Action_GizmoTranslation = Key_T,

		Action_Count
	};
	enum PrimitiveMode
	{
		PrimitiveMode_None,
		PrimitiveMode_Points,
		PrimitiveMode_Lines,
		PrimitiveMode_LineStrip,
		PrimitiveMode_LineLoop,
		PrimitiveMode_Triangles,
		PrimitiveMode_TriangleStrip
	};
	enum GizmoMode
	{
		GizmoMode_Translation,
		GizmoMode_Rotation,
		GizmoMode_Scale
	};
	struct CameraParam {
		bool   m_keyDown[Key_Count] = { false };
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
		float  m_snapTranslation = 0.0f;                    // Snap value for translation gizmos (world units). 0 = disabled.
		float  m_snapRotation = 0.0f;                    // Snap value for rotation gizmos (radians). 0 = disabled.
		float  m_snapScale = 0.0f;
		bool m_flipGizmoWhenBehind = true;

	};
	class Guizmo
	{
	public:

		Guizmo();


		void  begin(PrimitiveMode _mode);
		void  end();

		void  vertex(const Eigen::Vector3<float>& _position, float _size, Eigen::Vector4<uint8_t> _color);
		void  vertex(const Eigen::Vector3<float>& _position) { vertex(_position, sizeStack.back(), colorStack.back()); }

		void Draw(Eigen::Matrix4f& model);
		void DrawQuadFilled(const Eigen::Vector3<float>& _origin, const Eigen::Vector3<float>& _normal, const Eigen::Vector2<float>& _size);
		void DrawQuad(const Eigen::Vector3<float>& _a, const Eigen::Vector3<float>& _b, const Eigen::Vector3<float>& _c, const Eigen::Vector3<float>& _d);
		void DrawQuad(const Eigen::Vector3<float>& _origin, const Eigen::Vector3<float>& _normal, const Eigen::Vector2<float>& _size);
		void DrawTeapot(const Eigen::Matrix4f& model, const Eigen::Matrix4f& viewProj);
		void DrawQuadFilled(const Eigen::Vector3<float>& _a, const Eigen::Vector3<float>& _b, const Eigen::Vector3<float>& _c, const Eigen::Vector3<float>& _d);
		void DrawArrow(const Eigen::Vector3<float>& _start, const Eigen::Vector3<float>& _end, float _headLength = -1.0f, float _headThickness = -1.0f);
		void DrawSphereFilled(const Eigen::Vector3<float>& _origin, float _radius, int _detail = -1);
		bool DrawManpulate(unsigned _id, Eigen::Matrix4f& model);
		bool DrawManpulate(const char* _id, Eigen::Matrix4f& model);
		bool Translation(unsigned int _id, Eigen::Vector3<float>& _translation_, bool _local = false);
		bool gizmoPlaneTranslation_Behavior(const Eigen::Vector3<float>& _origin, const Eigen::Vector3<float>& _normal, float _snap, float _worldSize, Eigen::Vector3<float>* _out_);
		void gizmoAxisTranslation_Draw(unsigned int _id, const Eigen::Vector3<float>& _origin, const Eigen::Vector3<float>& _axis, float _worldHeight, float _worldSize, Eigen::Vector4<uint8_t> _color);
		bool gizmoAxisTranslation_Behavior(unsigned int _id, const Eigen::Vector3<float>& _origin, const Eigen::Vector3<float>& _axis, float _snap, float _worldHeight, float _worldSize, Eigen::Vector3<float>* _out_);
		void gizmoPlaneTranslation_Draw(unsigned int _id, const Eigen::Vector3<float>& _origin, const Eigen::Vector3<float>& _normal, float _worldSize, Eigen::Vector4<uint8_t> _color);
		bool gizmoPlaneTranslation_Behavior(unsigned int _id, const Eigen::Vector3<float>& _origin, const Eigen::Vector3<float>& _normal, float _snap, float _worldSize, Eigen::Vector3<float>* _out_);
		bool Rotation(unsigned int _id, Eigen::Matrix3f& _rotation_, bool _local = false);
		bool gizmoAxislAngle_Behavior(unsigned int _id, const Eigen::Vector3<float>& _origin, const Eigen::Vector3<float>& _axis, float _snap, float _worldRadius, float _worldSize, float* _out_);
		void gizmoAxislAngle_Draw(unsigned int _id, const Eigen::Vector3<float>& _origin, const Eigen::Vector3<float>& _axis, float _worldRadius, float _angle, Eigen::Vector4<uint8_t> _color, float _minAlpha);
		int  estimateLevelOfDetail(const Eigen::Vector3<float>& _position, float _worldSize, int _min = 4, int _max = 256);

		CameraParam& getCameraParam();
		void NewFrame();
		void EndFrame();

		void Submit();
		void Clear();
		bool MakeHot(unsigned int _id, float _depth, bool _intersects);
		void MakeActive(unsigned int _id);
		void ResetId();
		unsigned int MakeId(const char* _str);
		void  setId(unsigned int _id) { idStack.back() = _id; }
		unsigned int getId() const { return idStack.back(); }
		void pushId(unsigned int _id) { idStack.push_back(_id); }
		void popId() { assert(idStack.size() > 1); idStack.pop_back(); }
		void pushEnableSorting(bool _enable);
		void popEnableSorting();
		void setEnableSorting(bool _enable);
		bool getEnableSorting() const { return enableSortingStack.back(); }

		unsigned int getLayerId() const { return layerIdStack.back(); }
		void pushLayerId(unsigned int _layer);
		void popLayerId();
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
		std::vector<unsigned int>idStack;
		std::vector<bool>enableSortingStack;
		std::vector<unsigned int> layerIdStack;

		std::vector<Eigen::Matrix4f> matrixStack;
		CameraParam cameraParam;
		bool m_keyDownCurr[Key_Count];
		bool m_keyDownPrev[Key_Count];

		bool isKeyDown(Key _key) const { return m_keyDownCurr[_key]; }
		bool wasKeyPressed(Key _key) const { return m_keyDownCurr[_key] && !m_keyDownPrev[_key]; }

		unsigned int m_activeId;
		unsigned int m_hotId;
		float m_hotDepth;
		unsigned int m_appId;
		unsigned int m_appActiveId;
		unsigned int m_appHotId;
		Eigen::Vector3<float> m_gizmoStateVec3;
		Eigen::Matrix3f m_gizmoStateMat3;
		GizmoMode m_gizmoMode;
		bool m_gizmoLocal = false;         // Global mode selection for gizmos
		float gizmoHeightPixels = 64.0f;
		float gizmoSizePixels = 5.0f;
		float gizmoStateFloat;

		// Primitive state.
		PrimitiveMode       m_primMode;
		DrawPrimitiveType   m_primType;
		unsigned int  m_firstVertThisPrim;                // Index of the first vertex pushed during this primitive.
		unsigned int   m_vertCountThisPrim;                // # calls to vertex() since the last call to begin().
		Eigen::Vector3<float>  m_minVertThisPrim;
		Eigen::Vector3<float>  m_maxVertThisPrim;
		// Vertex data: one list per layer, per primitive type, *2 for sorted/unsorted.

		std::vector<VertexList*> m_vertexData[2];                    // Each layer is DrawPrimitive_Count consecutive lists.
		int m_vertexDataIndex;// 0, or 1 if sorting enabled.
		std::vector<unsigned int>m_layerIdMap;                       // Map Id -> vertex data index.
		int m_layerIndex; // Index of the currently active layer in m_layerIdMap.
		std::vector<DrawList> m_drawLists;                        // All draw lists for the current frame, available after calling endFrame() before calling reset().
		bool m_sortCalled;                       // Avoid calling sort() during every call to draw().
		bool m_endFrameCalled;                   // For assert, if vertices are pushed after endFrame() was called.

		int findLayerIndex(unsigned int _id) const;
		VertexList* getCurrentVertexList();
		const DrawList* getDrawLists() const { return m_drawLists.data(); }
		unsigned int getDrawListCount() const { return m_drawLists.size(); }
	};
}

