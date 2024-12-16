#include <QMouseEvent>
#include "viewerwindow.h"
#include "nodedatas/SurfaceMeshData.h"
#include "glloader.h"
#include "renderer/grid_renderer.h"
#include "renderer/Guizmo.h"
#include "core/read_mesh.h"
#include "algorithm/mesh_triangulate.h"
namespace MOON {
	//static float cursorX;
	//static float cursorY;
	static float viewW;
	static float viewH;
	Eigen::Matrix4f LookAt(const  Eigen::Vector3<float>& _from, const  Eigen::Vector3<float>& _to, const  Eigen::Vector3<float>& _up = Eigen::Vector3<float>(0.0f, 1.0f, 0.0f));
	static GridRenderer* grid_render;
	static Guizmo* guizmoRender;
	struct OpenGLProcAddressHelper {
		inline static QOpenGLContext* ctx;
		static void* getProcAddress(const char* name) {
			return (void*)ctx->getProcAddress(name);
		}
	};
	ViewerWindow* viewer_instance = nullptr;
	ViewerWindow::ViewerWindow(QWidget* parent) :
		QOpenGLWidget(parent)
	{
		if (viewer_instance == nullptr) {
			viewer_instance = this;
			//设置可以捕获鼠标移动消息
			this->setMouseTracking(true);
			this->grabKeyboard();
			//反锯齿
			QSurfaceFormat format;
			format.setSamples(4);
			this->setFormat(format);
		}
	}

	ViewerWindow::~ViewerWindow()
	{
	}

	void ViewerWindow::initializeGL()
	{
		// opengl funcs
		bool flag = initializeOpenGLFunctions();
		OpenGLProcAddressHelper::ctx = context();
		//CUSTOM_GL_API::CustomLoadGL(OpenGLProcAddressHelper::getProcAddress);
		CustomLoadGL(OpenGLProcAddressHelper::getProcAddress);

		viewer.append_mesh();
		viewer.append_mesh();
		viewer.append_mesh();
		viewer.append_mesh();
		viewer.append_mesh();
		viewer.append_mesh();
		viewer.append_mesh();
		viewer.init();
		//viewer.post_resize(800, 900);
		grid_render = new GridRenderer();
		guizmoRender = new Guizmo();
		//viewer.core().background_color = Eigen::Vector4f(1.0, 0.0, 0.0, 1.0);

		//开启计时器
		this->startTimer(16);
	}

	void ViewerWindow::timerEvent(QTimerEvent* e)
	{
		this->update();
	}



	void ViewerWindow::paintGL()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());
		guizmoRender->Clear();
		viewer.draw();


		const auto view = viewer.core().view;
		const auto viewinverse = view.inverse();
		const auto proj = viewer.core().proj;
		Eigen::Vector3<float> vieweye = Eigen::Vector3<float>(viewinverse(0, 3), viewinverse(1, 3), viewinverse(2, 3));
		Eigen::Vector3<float> viewforward = Eigen::Vector3<float>(viewinverse(0, 2), viewinverse(1, 2), viewinverse(2, 2)).normalized();
		Eigen::Matrix4f camWorld = viewinverse;

		// World space cursor ray from mouse position; for VR this might be the position/orientation of the HMD or a tracked controller.
		Eigen::Vector2<float> cursorPos = Eigen::Vector2<float>(viewer.current_mouse_x, viewer.current_mouse_y);
		//this->cursor().pos().x

		cursorPos = Eigen::Vector2<float>((cursorPos.x() / viewer.core().viewport(2)) * 2.0f - 1.0f, (cursorPos.y() / viewer.core().viewport(3)) * 2.0f - 1.0f);

		cursorPos(1) = -cursorPos(1); // window origin is top-left, ndc is bottom-left
		//printf("curpos(%f %f) viewport(%f,%f)\n", cursorPos.x(), cursorPos.y(), viewW, viewH);
		Eigen::Vector3<float> rayOrigin, rayDirection;
		if (viewer.core().orthographic)
		{
			rayOrigin(0) = cursorPos.x() / proj(0, 0);
			rayOrigin(1) = cursorPos.y() / proj(1, 1);
			rayOrigin(2) = 0.0f;
			Eigen::Vector4<float> it = camWorld * Eigen::Vector4<float>(rayOrigin.x(), rayOrigin.y(), rayOrigin.z(), 1.0f);
			rayOrigin = Eigen::Vector3<float>(it.x() / it.w(), it.y() / it.w(), it.z() / it.w());
			it = camWorld * Eigen::Vector4<float>(0.0f, 0.0f, -1.0f, 0.0f);
			rayDirection = Eigen::Vector3<float>(it.x() / it.w(), it.y() / it.w(), it.z() / it.w()); ;

		}
		else
		{
			rayOrigin = vieweye;
			rayDirection(0) = cursorPos.x() / proj(0, 0);
			rayDirection(1) = cursorPos.y() / proj(1, 1);
			rayDirection(2) = -1.0f;

			Eigen::Vector3<float>rayDirectionNormalized = rayDirection.normalized();
			Eigen::Vector4<float> it = camWorld * Eigen::Vector4<float>(rayDirectionNormalized.x(), rayDirectionNormalized.y(), rayDirectionNormalized.z(), 0.0f);
			rayDirection = Eigen::Vector3<float>(it.x(), it.y(), it.z());
			//printf("Ray Direction(%f,%f,%f)\n", rayDirection.x(), rayDirection.y(), rayDirection.z());

		}
		CameraParam& cameraParam = guizmoRender->getCameraParam();
		cameraParam.viewportWidth = viewer.core().viewport.z();
		cameraParam.viewportHeight = viewer.core().viewport.w();
		cameraParam.projectY = tan(90.0 / 360.0 * PI) * 2;
		cameraParam.orthProj = false;
		cameraParam.eye = vieweye;
		cameraParam.viewDirectioin = viewforward;
		cameraParam.rayOrigin = rayOrigin;
		cameraParam.rayDirection = rayDirection;
		cameraParam.view = view;
		cameraParam.proj = proj;
		cameraParam.viewProj = proj * view;
		//printf("view Direction(%f,%f,%f)\n", viewforward.x(), viewforward.y(), viewforward.z());
		cameraParam.m_keyDown[Mouse_Left] = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
		bool ctrlDown = (GetAsyncKeyState(VK_LCONTROL) & 0x8000) != 0;
		cameraParam.m_keyDown[Key_L] = ctrlDown && (GetAsyncKeyState(0x4c) & 0x8000) != 0;
		cameraParam.m_keyDown[Key_T] = ctrlDown && (GetAsyncKeyState(0x54) & 0x8000) != 0;
		cameraParam.m_keyDown[Key_R] = ctrlDown && (GetAsyncKeyState(0x52) & 0x8000) != 0;
		cameraParam.m_keyDown[Key_S] = ctrlDown && (GetAsyncKeyState(0x53) & 0x8000) != 0;

		cameraParam.m_snapTranslation = ctrlDown ? 0.5f : 0.0f;
		cameraParam.m_snapRotation = ctrlDown ? 0.523f : 0.0f;
		cameraParam.m_snapScale = ctrlDown ? 0.5f : 0.0f;

		guizmoRender->NewFrame();
		static Eigen::Matrix4f model = Eigen::Matrix4f::Identity();


		//guizmoRender->Draw(model);
		guizmoRender->DrawManpulate("Mode", model);
		guizmoRender->DrawTeapot(model, proj * view);
		//guizmoRender->Submit();
		guizmoRender->EndFrame();
		//grid_render->DrawGrid(viewer.core().camera_eye, proj * view);


	}

	void ViewerWindow::leaveEvent(QEvent* event)
	{
	}

	void ViewerWindow::resizeEvent(QResizeEvent* event)
	{

		QOpenGLWidget::resizeEvent(event);
		viewer.post_resize(event->size().width() * 1.5, event->size().height() * 1.5);

		viewW = event->size().width();
		viewH = event->size().height();
	}

	void ViewerWindow::mousePressEvent(QMouseEvent* e)
	{
		if (blockMouseMessage) {
			return;
		}
		Qt::MouseButton mb = e->button();
		if (mb == Qt::MouseButton::LeftButton)
			viewer.mouse_down(Viewer::MouseButton::Left);
		else if (mb == Qt::MouseButton::MiddleButton)
			viewer.mouse_down(Viewer::MouseButton::Middle);
		else if (mb == Qt::MouseButton::RightButton)
			viewer.mouse_down(Viewer::MouseButton::Right);
	}

	void ViewerWindow::mouseMoveEvent(QMouseEvent* event)
	{

		auto pos = event->localPos();
		//cursorX = pos.x() * 1.5;
		//cursorY = pos.y() * 1.5;
		viewer.mouse_move(pos.x() * 1.5, pos.y() * 1.5);
	}

	void ViewerWindow::mouseReleaseEvent(QMouseEvent* event)
	{
		if (blockMouseMessage) {
			return;
		}
		Qt::MouseButton mb = event->button();
		if (mb == Qt::MouseButton::LeftButton)
			viewer.mouse_up(Viewer::MouseButton::Left);
		else if (mb == Qt::MouseButton::MiddleButton)
			viewer.mouse_up(Viewer::MouseButton::Middle);
		else if (mb == Qt::MouseButton::RightButton)
			viewer.mouse_up(Viewer::MouseButton::Right);
	}

	void ViewerWindow::wheelEvent(QWheelEvent* event)
	{
		viewer.mouse_scroll(event->angleDelta().y());
	}
	void ViewerWindow::keyPressEvent(QKeyEvent* event)
	{
		blockMouseMessage = true;
		if (event->key() == Qt::Key::Key_Space) {

		}
	}
	void ViewerWindow::keyReleaseEvent(QKeyEvent* event)
	{
		if (event->key() == Qt::Key::Key_Space) {
			blockMouseMessage = false;
		}
	}
	void ViewerWindow::viewnode(const std::shared_ptr<NodeData>& node) {
		auto mesh_data = std::dynamic_pointer_cast<SurfaceMeshData>(node);

		bool is_triangle = mesh_data->mesh()->is_triangle_mesh();
		SurfaceMesh* present_mesh = mesh_data->mesh().get();
		if (!is_triangle) {
			present_mesh = new SurfaceMesh(*mesh_data->mesh());
			triangulate(*present_mesh);
		}
		Eigen::MatrixXd SV;
		auto& pos = present_mesh->positions();
		SV.resize(pos.size(), 3);
		for (int i = 0; i < pos.size(); i++) {
			SV.row(i) << pos[i][0], pos[i][1], pos[i][2];
		}
		Eigen::MatrixXi SF;
		SF.resize(present_mesh->faces_size(), 3);
		const auto& faces = present_mesh->faces();
		int j = 0, x, y, z;
		for (auto f : faces) {
			auto it = present_mesh->vertices(f).begin();
			x = (*it++).idx();
			y = (*it++).idx();
			z = (*it++).idx();
			SF.row(j++) << x, y, z;
		}
		viewer.data(0).clear();
		viewer.data(0).set_mesh(SV, SF);
	}

}