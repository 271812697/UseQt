#ifndef GLWIDGET_H
#define GLWIDGET_H
//qt
#include <QOpenGLWidget>
//#include <QOpenGLFunctions>
#include<QOpenGLFunctions_4_5_Core>

#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

//glm
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include <glm/gtx/rotate_vector.hpp>

//数据类型
typedef glm::ivec2 vec2i;
typedef glm::ivec3 vec3i;
typedef glm::ivec4 vec4i;
typedef glm::vec2 vec2f;
typedef glm::vec3 vec3f;
typedef glm::vec4 vec4f;

//顶点类型
class vertex
{
public:
	vec3f position;
	vec3f normal;
	vec4f color;
	vec2f texcoord;

	vertex() : position(), normal(), color(), texcoord() {}

	vertex(const vec3f& pos,
		const vec3f& n = vec3f(),
		const vec4f& c = vec4f(1.0f),
		const vec2f& uv = vec2f())
		: position(pos), normal(n), color(c), texcoord(uv)
	{
	}
};

//模型
class GLMesh
{
public:
	std::vector<vertex> vbo;    //顶点缓冲
	std::vector<uint32_t> ebo;  //索引缓冲

	//构造函数
	GLMesh() : vbo(), ebo()
	{

	}

	//清空
	void clear()
	{
		vbo.clear();
		ebo.clear();
	}

	//初始化球体
	void initSphere(int width, int height, float size)
	{
		//初始化顶点
		float coord_x_step = 1.0f / width;
		float coord_y_step = 1.0f / height;

		float x_step = 360.0f / (width - 1);
		float y_step = 180.0f / (height - 1);

		float angle_x, angle_y;
		float sine_x, cosine_x;
		float latRadius;

		vec4f color(1.0);

		vbo.resize(width * height);
		vertex* vs = vbo.data();
		for (int y = 0; y < height; ++y) {
			angle_x = glm::radians(y * y_step);
			sine_x = sin(angle_x);
			cosine_x = cos(angle_x);
			latRadius = sine_x * size;
			for (int x = 0; x < width; ++x)
			{
				angle_y = glm::radians(x * x_step);
				vs[x].position.x = -cos(angle_y) * latRadius;
				vs[x].position.y = cosine_x * size;
				vs[x].position.z = sin(angle_y) * latRadius;
				vs[x].texcoord.x = x * coord_x_step;
				vs[x].texcoord.y = y * coord_y_step;
				vs[x].color = color;
				vs[x].normal = glm::normalize(vs[x].position);
			}
			vs += width;
		}

		//初始化索引
		const int TRIANGLE_VERTICES = 6;

		ebo.resize((width - 1) * (height - 1) * TRIANGLE_VERTICES);
		uint32_t* es = ebo.data();
		int k = 0;
		for (int y = 0; y < height - 1; ++y) {
			for (int x = 0; x < width - 1; ++x) {
				es[0] = k + x;
				es[1] = k + width + x;
				es[2] = k + width + x + 1;

				es[3] = k + x;
				es[4] = k + width + x + 1;
				es[5] = k + x + 1;

				es += TRIANGLE_VERTICES;
			}
			k += width;
		}
	}

};

class GLWidget : public QOpenGLWidget, QOpenGLFunctions_4_5_Core
{
	Q_OBJECT
public:
	explicit GLWidget(QWidget* parent = nullptr);
	~GLWidget();

	//初始化球体
	void initSphere();

	//控件事件

	void mousePressEvent(QMouseEvent* e) override;
	void mouseReleaseEvent(QMouseEvent* e) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* event)override;

	void timerEvent(QTimerEvent* e) override;


	void initializeGL() override;
	void resizeGL(int w, int h) override;
	void paintGL() override;

public slots:
	void sliderValueChange(int n);

public:
	QOpenGLShaderProgram program;
	vec4f color;

private:
	QMatrix4x4 projection;
	QMatrix4x4 view;
	QMatrix4x4 model;

	GLMesh mesh;                //球体
	QOpenGLTexture* texSun;     //太阳纹理
	QOpenGLTexture* texEarth;   //地球纹理

	vec2i mouse;                //鼠标位置
	vec2i lastmouse;            //鼠标的最后位置
	bool mousedown;             //鼠标是否按下
	Qt::MouseButton button;     //鼠标按键

	vec3f viewPosition;         //摄像头移动
	vec3f viewRotate;           //摄像头旋转

	float sunSelfRotate;        //太阳自转
	float earthRotate;          //地球公转
	float earthSelfRotate;      //地球自转

	float m_speed;              //旋转速度

	void showinfo();


public:
	//绘制顶点列表
	void draw_arrays(GLenum shape, const vertex* vs, size_t pos, size_t size)
	{
		vertex_begin(vs);
		glDrawArrays(shape, pos, size);

		vertex_end();
	}

	void draw_arrays(GLenum shape, const std::vector<vertex>& vs)
	{
		vertex_begin(vs.data());

		glDrawArrays(shape, 0, vs.size());
		vertex_end();
	}

	//绘制索引的顶点列表
	void draw_elements(GLenum shape, const vertex* vs, const GLuint* es, size_t size)
	{
		vertex_begin(vs);
		glDrawElements(shape, size, GL_UNSIGNED_INT, es);
		vertex_end();
	}

	void draw_elements(GLenum shape, const std::vector<vertex>& vs, const std::vector<uint32_t>& es)
	{
		vertex_begin(vs.data());
		glDrawElements(shape, es.size(), GL_UNSIGNED_INT, es.data());
		vertex_end();
	}

	void draw_line(float x1, float y1, float z1, float x2, float y2, float z2)
	{
		vertex vtx[] = {
			vertex(vec3f(x1, y1, z1), vec3f(), color, vec2f()),
			vertex(vec3f(x2, y2, z2), vec3f(), color, vec2f()),
		};

		draw_arrays(GL_LINES, vtx, 0, 2);
	}

protected:
	//顶点属性设置
	void vertex_begin(const vertex* vs)
	{

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		glEnableVertexAttribArray(3);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), glm::value_ptr(vs->position));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertex), glm::value_ptr(vs->normal));
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(vertex), glm::value_ptr(vs->color));
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(vertex), glm::value_ptr(vs->texcoord));
	}

	void vertex_end()
	{
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
	}
};

#endif // GLWIDGET_H
