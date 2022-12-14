#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"clock.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->startTimer(16);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::timerEvent(QTimerEvent *)
{


    this->setWindowTitle(QString::asprintf("FPS:%f,%fms",Clock::fps,Clock::ms));
}

