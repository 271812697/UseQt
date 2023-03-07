#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QEvent>
#include<QKeyEvent>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    void erase();
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent*event);
    void keyPressEvent(QKeyEvent*event);
    bool is(QPoint base);
private slots:
    void drop();
private:
    void init();
    QTimer* T;
    QPoint base;
    int cur;
    int next;
    QFont F;
    int score=0;
    QPoint base1;
    void move(int);


    Ui::MainWindow *ui;
   void temp(QPainter* ,QPoint,int index);
};
#endif // MAINWINDOW_H
