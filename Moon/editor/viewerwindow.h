#pragma once
#include <QOpenGLWidget>
#include<QOpenGLFunctions_4_5_Core>

namespace MOON {

	class Editor;
	class ViewerWindow : public QOpenGLWidget, QOpenGLFunctions_4_5_Core
	{
		Q_OBJECT
	public:
		explicit ViewerWindow(QWidget* parent);
		~ViewerWindow();


		void initializeGL() override;
		void timerEvent(QTimerEvent* e) override;
		void resizeGL(int w, int h) override;

		void paintGL() override;

		void leaveEvent(QEvent* event) override;

		void resizeEvent(QResizeEvent* event) override;

		void mousePressEvent(QMouseEvent* event) override;

		void mouseMoveEvent(QMouseEvent* event) override;

		void mouseReleaseEvent(QMouseEvent* event) override;

		void wheelEvent(QWheelEvent* event) override;


	};
}