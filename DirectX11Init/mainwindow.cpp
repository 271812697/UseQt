#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer=new QTimer(this);
    timer->start();
    timer->setInterval(10);
    connect(timer,SIGNAL(timeout()),ui->widget,SLOT(RenderUpdate()));
    connect(timer,SIGNAL(timeout()),ui->widget_2,SLOT(RenderUpdate()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

