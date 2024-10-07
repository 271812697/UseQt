#include "glwidget.h"
#include <QMainWindow>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>

GLWidget::GLWidget(QWidget* parent) :
	QOpenGLWidget(parent)
{
	//设置可以捕获鼠标移动消息
	this->setMouseTracking(true);

	projection.setToIdentity();
	view.setToIdentity();
	model.setToIdentity();

	viewPosition = vec3f(0.0f, 0.0f, -20.0f);
	viewRotate = vec3f(0.0f);

	mousedown = false;

	texSun = nullptr;
	texEarth = nullptr;

	//初始化数据
	sunSelfRotate = 0.0f;
	earthRotate = 0.0f;
	earthSelfRotate = 0.0f;

	m_speed = 1.0f;

	//反锯齿
	QSurfaceFormat format;
	format.setSamples(4);
	this->setFormat(format);
}

GLWidget::~GLWidget()
{
	mesh.clear();

	if (texSun) {
		delete texSun;
		texSun = nullptr;
	}

	if (texEarth) {
		delete texEarth;
		texEarth = nullptr;
	}
}

void GLWidget::initSphere()
{
}

//鼠标按下
void GLWidget::mousePressEvent(QMouseEvent* e)
{
	//记录鼠标信息
	mousedown = true;
	button = e->button();
	lastmouse = vec2i(e->localPos().x(), e->localPos().y());

}

//鼠标弹起
void GLWidget::mouseReleaseEvent(QMouseEvent*)
{
	mousedown = false;
}

//鼠标移动
void GLWidget::mouseMoveEvent(QMouseEvent* e)
{
	// Save mouse press position
	mouse = vec2i(e->localPos().x(), e->localPos().y());

	if (mousedown) {
		float ox = mouse.x - lastmouse.x;
		float oy = mouse.y - lastmouse.y;

		if (button == Qt::LeftButton) {
			float scale = 0.1;//缩放率
			viewRotate.y += ox * scale;
			viewRotate.x += oy * scale;
		}
		else if (button == Qt::RightButton) {
			ox /= this->width() / fabs(viewPosition.z);
			oy /= this->height() / fabs(viewPosition.z);
			viewPosition.x += ox;
			viewPosition.y -= oy;
		}

		lastmouse = mouse;
	}

}

//鼠标滚动
void GLWidget::wheelEvent(QWheelEvent* event)
{

	if (event->angleDelta().y() < 0) {
		viewPosition.z *= 1.1;
	}
	else {
		viewPosition.z *= 0.9;
		if (viewPosition.z > -1.0) {
			viewPosition.z = -1.0;
		}
	}

}

//计时器事件
void GLWidget::timerEvent(QTimerEvent*)
{
	//星球旋转
	sunSelfRotate += 0.5f * m_speed;
	earthSelfRotate += 12.0f * m_speed;
	earthRotate += 1.0f * m_speed;

	this->update();
}


//opengl初始化
void GLWidget::initializeGL()
{
	//初始化一下扩展函数
	initializeOpenGLFunctions();

	//初始化shader
	program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/res/texture.vs");
	program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/res/texture.fs");
	program.link();
	program.bind();

	//设置采用通道
	program.setUniformValue("texture_id", 0);

	program.release();

	//初始化球体
	mesh.initSphere(20, 20, 1.0f);

	//启用纹理
	//::glEnable(GL_TEXTURE_2D);
	texSun = new QOpenGLTexture(QImage(":res/sun.png"));
	texEarth = new QOpenGLTexture(QImage(":res/earth.png"));

	//开启计时器
	this->startTimer(16);
}

//控件大小改变
void GLWidget::resizeGL(int w, int h)
{
	qDebug() << "控件大小：\n" << w << "x" << h;

	if (w == 0 || h == 0) {
		return;
	}

	glViewport(0, 0, w, h);
	projection.setToIdentity();
	projection.perspective(60.0f, float(w) / float(h), 1.0f, 10000.0f);
}

//绘制事件
void GLWidget::paintGL()
{
	//清屏
	glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//参数设置
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//启用混合
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//启用渲染器
	program.bind();

	//鼠标控制的视口
	view.setToIdentity();
	view.translate(viewPosition.x, viewPosition.y, viewPosition.z);
	view.rotate(viewRotate.x, 1.0, 0.0, 0.0);
	view.rotate(viewRotate.y, 0.0, 1.0, 0.0);
	view.rotate(viewRotate.z, 0.0, 0.0, 1.0);

	//mvp矩阵
	QMatrix4x4 mvp;

	//启用纹理
	program.setUniformValue("texture_mode", true);

	//----------------------------------------------------------
	//绘制太阳
	model.setToIdentity();

	//自转
	model.rotate(sunSelfRotate, 0.0f, 1.0f, 0.0f);

	mvp = projection * view * model;
	program.setUniformValue("mvp", mvp);
	program.setUniformValue("model", model);

	texSun->bind();
	draw_elements(GL_TRIANGLES, mesh.vbo, mesh.ebo);
	texSun->release();

	//----------------------------------------------------------
	//绘制地球
	const float length = 5.0f;

	model.setToIdentity();

	//公转
	model.rotate(earthRotate, 0.0f, 1.0f, 0.0f);

	//距离太远距离
	model.translate(length, 0.0f, 0.0f);

	//自转
	model.rotate(-earthRotate, 0.0f, 1.0f, 0.0f);

	//地球倾斜23.5度
	model.rotate(23.5f, 0.0f, 0.0f, 1.0f);

	//自转
	model.rotate(earthSelfRotate, 0.0f, 1.0f, 0.0f);

	//地球要小一点
	model.scale(0.25f, 0.25f, 0.25f);

	mvp = projection * view * model;
	program.setUniformValue("mvp", mvp);
	program.setUniformValue("model", model);

	texEarth->bind();
	draw_elements(GL_TRIANGLES, mesh.vbo, mesh.ebo);
	texEarth->release();

	//----------------------------------------------------------
	//绘制地球轨迹

	//关闭纹理
	program.setUniformValue("texture_mode", false);

	//绘制地球自转轴，这个用到前面的矩阵
	color = vec4f(1.0f, 1.0f, 0.0f, 0.25f);
	draw_line(0.0f, -2.0f, 0.0f, 0.0f, 2.0f, 0.0f);

	//生成圆
	std::vector<vertex> vs;
	vs.resize(360);
	vec2f p = vec2f(length, 0.0f);
	for (int i = 0; i < 360; ++i) {
		p = glm::rotate(p, glm::radians(1.0f));
		vs[i].position = vec3f(p.x, 0.0f, p.y);
		vs[i].color = vec4f(1.0f, 1.0f, 0.0f, 0.25f);//黄色轨迹线
	}

	//重置矩阵
	model.setToIdentity();
	mvp = projection * view * model;
	program.setUniformValue("mvp", mvp);
	program.setUniformValue("model", model);

	draw_arrays(GL_LINE_LOOP, vs);//绘制连线

	//绘制坐标系
	/*
	//绘制一个坐标系
	color = vec4f(1.0f, 0.0f, 0.0f, 1.0f);
	draw_line(0.0f, 0.0f, 0.0f, 10000.0f, 0.0f, 0.0f);

	color = vec4f(0.0f, 1.0f, 0.0f, 1.0f);
	draw_line(0.0f, 0.0f, 0.0f, 0.0f, 10000.0f, 0.0f);

	color = vec4f(0.0f, 0.0f, 1.0f, 1.0f);
	draw_line(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -10000.0f);
	//*/

	//停止使用渲染器
	program.release();

	//显示信息
	showinfo();
}

void GLWidget::showinfo()
{
	char buf[1024];

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_DEPTH_TEST);

	QPainter dc(this);
	dc.setFont(QFont("微软雅黑", 16));

	dc.setPen(Qt::white);

	//文字输出信息
	snprintf(buf, 1024, "mouse    : %d, %d", mouse.x, mouse.y);
	dc.drawText(10, 30, buf);

	snprintf(buf, 1024, "position : %0.2f, %0.2f, %0.2f", viewPosition.x, viewPosition.y, viewPosition.z);
	dc.drawText(10, 60, buf);

	snprintf(buf, 1024, "rotate   : %0.2f, %0.2f, %0.2f", viewRotate.x, viewRotate.y, viewRotate.z);
	dc.drawText(10, 90, buf);

	snprintf(buf, 1024, "speed    : %0.2f", m_speed);
	dc.drawText(10, 120, buf);
}


void GLWidget::sliderValueChange(int n)
{
	m_speed = n / 100.0f;
}
