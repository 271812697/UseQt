#include "widget.h"
#include "ui_widget.h"
#include "screenwidget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("Screen");
}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked()
{
    ScreenWidget::Instance()->showFullScreen();

}


void Widget::on_pushButton_2_clicked()
{

}


void Widget::on_pushButton_clicked(bool checked)
{

}


void Widget::on_pushButton_3_released()
{

}

