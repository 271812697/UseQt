#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"clock.h"
namespace scene {
 extern float skybox_exposure ;
 extern float skybox_lod ;
 extern QStringList mingyueAnimation;
 extern QStringList zhaolingerAnimation;
 extern size_t* curmingyue;
 extern size_t* curzhaolinger;
}
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


void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    scene::skybox_exposure=float(value)/10.0f;
}


void MainWindow::on_horizontalSlider_2_valueChanged(int value)
{
    scene::skybox_lod=value;
}


void MainWindow::on_pushButton_clicked()
{

    ui->comboBox->addItems(scene::zhaolingerAnimation);
    ui->comboBox_2->addItems(scene::mingyueAnimation);
}


void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    *scene::curzhaolinger=index;
}


void MainWindow::on_comboBox_2_currentIndexChanged(int index)
{
    *scene::curmingyue=index;
}

