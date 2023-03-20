#ifndef SCREENWIDGET_H
#define SCREENWIDGET_H

#include <QWidget>
#include <QMenu>
#include <QPoint>
#include <QSize>


class ScreenWidget : public QWidget
{
private:
   static QScopedPointer<ScreenWidget> self;
   bool mousedown=false;
   bool inRect=false;
   QPoint startPos;          //坐标
   QPoint endPos;          //坐标
   QPoint movPos;          //坐标
   bool InRect(const QPoint& pos);
   bool Move(const QPoint& delta);
    Q_OBJECT
public:

    QPixmap *fullScreen;    //保存全屏图像
    QPixmap *bgScreen;      //模糊背景图
    static ScreenWidget* Instance();
    explicit ScreenWidget(QWidget *parent = nullptr);
    QMenu *menu;            //右键菜单对象
protected:
    void contextMenuEvent(QContextMenuEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    void showEvent(QShowEvent *);

private slots:

    void saveScreen();
    void saveFullScreen();
    void saveScreenOther();
    void saveFullOther();
};

#endif // SCREENWIDGET_H
