#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>      //布局
#include <QVBoxLayout>      //布局
#include <QSlider>          //滑块控件
#include"form.h"
#include "glwidget.h"

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);
	Form winform;
	winform.show();

	//生成一个主窗口
	QWidget window;
	window.resize(800, 600);

	//布局
	QVBoxLayout* layout = new QVBoxLayout(&window);

	//layout->setMargin(2);

	//自定义OpenGL控件
	GLWidget* gl = new GLWidget(&window);
	layout->addWidget(gl);

	//滑块
	QSlider slider;
	//100为一个单位，速度是value的1/100
	slider.setRange(10, 1000);
	slider.setValue(100);
	slider.setOrientation(Qt::Horizontal);

	slider.connect(&slider, SIGNAL(valueChanged(int)), gl, SLOT(sliderValueChange(int)));

	layout->addWidget(&slider);

	//设置布局，显示窗口
	window.setLayout(layout);
	window.show();

	return a.exec();
}
