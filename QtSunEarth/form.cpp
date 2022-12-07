#include "form.h"
#include "ui_form.h"
#include <QDebug>

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    this->layout()->setMargin(2);
}

Form::~Form()
{
    delete ui;
}

void Form::mousePressEvent(QMouseEvent *e)
{
    qDebug() << "mouse press"<<Qt::endl;

}

void Form::mouseReleaseEvent(QMouseEvent *e)
{

}

void Form::mouseMoveEvent(QMouseEvent *event)
{

}

void Form::wheelEvent(QWheelEvent *event)
{

}
