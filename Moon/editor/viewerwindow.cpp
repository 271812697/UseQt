#include <QMouseEvent>
#include "viewerwindow.h"
#include "nodedatas/SurfaceMeshData.h"
#include "glloader.h"
#include "renderer/grid_renderer.h"
#include "core/read_mesh.h"
#include "algorithm/mesh_triangulate.h"
namespace MOON {
	static GridRenderer* grid_render;
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
		viewer.draw();
		const auto model = viewer.core().view;
		const auto proj = viewer.core().proj;
		grid_render->DrawGrid(viewer.core().camera_eye, proj * model);
	}

	void ViewerWindow::leaveEvent(QEvent* event)
	{
	}

	void ViewerWindow::resizeEvent(QResizeEvent* event)
	{
		QOpenGLWidget::resizeEvent(event);
		viewer.post_resize(event->size().width() * 1.50, event->size().height() * 1.50);
		//viewer.post_resize(event->size().width(), event->size().height());
	}

	void ViewerWindow::mousePressEvent(QMouseEvent* e)
	{
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
		auto pos = event->windowPos();
		viewer.mouse_move(pos.x(), pos.y());
	}

	void ViewerWindow::mouseReleaseEvent(QMouseEvent* event)
	{
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