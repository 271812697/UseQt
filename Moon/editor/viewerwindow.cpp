#include "viewerwindow.h"
#include "glloader.h"
namespace MOON {

	struct OpenGLProcAddressHelper {
		inline static QOpenGLContext* ctx;
		static void* getProcAddress(const char* name) {
			return (void*)ctx->getProcAddress(name);
		}
	};

	ViewerWindow::ViewerWindow(QWidget* parent) :
		QOpenGLWidget(parent)
	{
		//���ÿ��Բ�������ƶ���Ϣ
		this->setMouseTracking(true);
		//�����
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
		//������ʱ��
		//this->startTimer(16);
	}

	void ViewerWindow::timerEvent(QTimerEvent* e)
	{
		this->update();
	}

	void ViewerWindow::resizeGL(int w, int h)
	{
		if (w == 0 || h == 0) {
			return;
		}

		glViewport(0, 0, w, h);
	}

	void ViewerWindow::paintGL()
	{


		//����
		glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());

		glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//��������
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		//���û��
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	}

	void ViewerWindow::leaveEvent(QEvent* event)
	{
	}

	void ViewerWindow::resizeEvent(QResizeEvent* event)
	{
		QOpenGLWidget::resizeEvent(event);
	}

	void ViewerWindow::mousePressEvent(QMouseEvent* event)
	{
	}

	void ViewerWindow::mouseMoveEvent(QMouseEvent* event)
	{
	}

	void ViewerWindow::mouseReleaseEvent(QMouseEvent* event)
	{
	}

	void ViewerWindow::wheelEvent(QWheelEvent* event)
	{
	}

}