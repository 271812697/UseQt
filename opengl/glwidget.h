#ifndef GLWIDGET_H
#define GLWIDGET_H
#include"asset/all.h"
#include"component/all.h"

//qt
#include <QOpenGLWidget>
#include<QOpenGLFunctions_4_5_Core>
#include"qopenglfunctions_4_5_compatibility.h"
//数据类型
typedef glm::ivec2 vec2i;
typedef glm::ivec3 vec3i;
typedef glm::ivec4 vec4i;
typedef glm::vec2 vec2f;
typedef glm::vec3 vec3f;
typedef glm::vec4 vec4f;
class GLWidget : public QOpenGLWidget, QOpenGLFunctions_4_5_Core
{
	Q_OBJECT

public:
	explicit GLWidget(QWidget* parent = nullptr);
	~GLWidget();

	glm::ivec2 GetResolution() {
		return { this->width(),this->height() };
	}

	//控件事件
	void keyPressEvent(QKeyEvent* event)override;
	void keyReleaseEvent(QKeyEvent* event)override;
	void mousePressEvent(QMouseEvent* e) override;
	void mouseReleaseEvent(QMouseEvent* e) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* event)override;

	void timerEvent(QTimerEvent* e) override;


	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;
	float Aspect();

public slots:
	void sliderValueChange(int n);

public:
	asset::Shader* shader;
private:
	component::Mesh mesh;                //球体
	asset::Texture* texSun;     //太阳纹理
	asset::Texture* texEarth;   //地球纹理
	component::Camera camera;
	float m_speed;              //旋转速度
	void showinfo();
};

#endif // GLWIDGET_H
