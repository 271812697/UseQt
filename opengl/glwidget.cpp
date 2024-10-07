#include "glwidget.h"
#include"input.h"
#include <QMainWindow>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include"clock.h"
#include"win_dow.h"
#include"example/scene01.h"
scene::Scene* cur = nullptr;
unsigned defaultFrame;
//gl单例
QOpenGLFunctions_4_5_Core* GLFuncInstance = nullptr;
QOpenGLFunctions_4_5_Core** getGlInstanceDPtr() {
	return &GLFuncInstance;
}
GLWidget::GLWidget(QWidget* parent) :
	QOpenGLWidget(parent)
{

	GLFuncInstance = this;
	//设置可以捕获鼠标移动消息
	this->setMouseTracking(true);
	this->grabKeyboard();
	texSun = nullptr;
	texEarth = nullptr;
	//反锯齿
	QSurfaceFormat format;
	format.setSamples(4);
	this->setFormat(format);
}

GLWidget::~GLWidget()
{
	if (cur) {
		delete cur;
	}

	if (texSun) {
		delete texSun;
		texSun = nullptr;
	}

	if (texEarth) {
		delete texEarth;
		texEarth = nullptr;
	}
}

void GLWidget::keyPressEvent(QKeyEvent* event)
{
	unsigned char _key = '0';
	switch (event->key()) {
	case Qt::Key_W: case Qt::Key_Up:_key = 'w'; break;
	case Qt::Key_S: case Qt::Key_Down:_key = 's'; break;
	case Qt::Key_A: case Qt::Key_Left:_key = 'a'; break;
	case Qt::Key_D: case Qt::Key_Right:_key = 'd'; break;
	case Qt::Key_Q: _key = 'q'; break;
	case Qt::Key_E: _key = 'e'; break;

	default:
		return;
	}

	Input::SetKeyDown(_key, true);

}

void GLWidget::keyReleaseEvent(QKeyEvent* event)
{
	unsigned char _key = '0';
	switch (event->key()) {
	case Qt::Key_W: case Qt::Key_Up:_key = 'w'; break;
	case Qt::Key_S: case Qt::Key_Down:_key = 's'; break;
	case Qt::Key_A: case Qt::Key_Left:_key = 'a'; break;
	case Qt::Key_D: case Qt::Key_Right:_key = 'd'; break;
	case Qt::Key_Q: _key = 'q'; break;
	case Qt::Key_E: _key = 'e'; break;

	default:
		return;
	}

	Input::SetKeyDown(_key, false);
}

//鼠标按下
void GLWidget::mousePressEvent(QMouseEvent* e)
{


	Qt::MouseButton mb = e->button();
	MouseButton bt = MouseButton::Right;
	if (mb == Qt::MouseButton::LeftButton)
		bt = MouseButton::Left;
	else if (mb == Qt::MouseButton::MiddleButton)
		bt = MouseButton::Middle;
	else if (mb == Qt::MouseButton::RightButton)
		bt = MouseButton::Right;
	Input::SetMouseDown(bt, true);

}

//鼠标弹起
void GLWidget::mouseReleaseEvent(QMouseEvent* e)
{

	Qt::MouseButton mb = e->button();
	MouseButton bt = MouseButton::Right;
	if (mb == Qt::MouseButton::LeftButton)
		bt = MouseButton::Left;
	else if (mb == Qt::MouseButton::MiddleButton)
		bt = MouseButton::Middle;
	else if (mb == Qt::MouseButton::RightButton)
		bt = MouseButton::Right;
	Input::SetMouseDown(bt, false);
}

//鼠标移动
void GLWidget::mouseMoveEvent(QMouseEvent* e)
{
	auto pos = e->windowPos();

	Input::SetCursor(pos.x(), pos.y());


}

//鼠标滚动
void GLWidget::wheelEvent(QWheelEvent* event)
{
	Input::SetScroll(event->angleDelta().y());
}

//计时器事件
void GLWidget::timerEvent(QTimerEvent*)
{
	Clock::Update();
	this->update();
}


//opengl初始化
void GLWidget::initializeGL()
{
	//开启计时器
	this->startTimer(6);
	//初始化一下扩展函数
	initializeOpenGLFunctions();

	resizeGL(this->width(), this->height());
	cur = new scene::scene01("01");

	cur->Init();

	defaultFrame = defaultFramebufferObject();
	CORE_INFO("default FrameBuffer is {0}", defaultFrame);

}

//控件大小改变
void GLWidget::resizeGL(int w, int h)
{
	if (w == 0 || h == 0) {
		return;
	}
	Window::Resize(w, h);
	glViewport(0, 0, w, h);
}

//绘制事件
void GLWidget::paintGL()
{
	defaultFrame = defaultFramebufferObject();
	cur->OnImGuiRender();
	cur->OnSceneRender();
	//显示信息
	showinfo();
}
void GLWidget::showinfo()
{


}
void GLWidget::sliderValueChange(int n)
{
	m_speed = n / 100.0f;
}
