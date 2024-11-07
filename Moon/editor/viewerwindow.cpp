#include <QMouseEvent>
#include "viewerwindow.h"
#include "glloader.h"
#include "renderer/grid_renderer.h"
namespace MOON {
	static GridRenderer* grid_render;
	struct OpenGLProcAddressHelper {
		inline static QOpenGLContext* ctx;
		static void* getProcAddress(const char* name) {
			return (void*)ctx->getProcAddress(name);
		}
	};

	ViewerWindow::ViewerWindow(QWidget* parent) :
		QOpenGLWidget(parent)
	{
		//设置可以捕获鼠标移动消息
		this->setMouseTracking(true);
		//反锯齿
		QSurfaceFormat format;
		format.setSamples(4);
		this->setFormat(format);
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


		//清屏
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
		viewer.mouse_move(pos.x() * 1.5, pos.y() * 1.5);
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

}