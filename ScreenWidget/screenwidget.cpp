#include "screenwidget.h"
#include "qmutex.h"
#include "qapplication.h"
#include "qpainter.h"
#include "qfiledialog.h"
#include "qevent.h"
#include "qdatetime.h"
#include "qstringlist.h"
#include "qdebug.h"

#include "qscreen.h"
#define STRDATETIME qPrintable (QDateTime::currentDateTime().toString("yyyy-MM-dd-HH-mm-ss"))

QScopedPointer<ScreenWidget> ScreenWidget::self;

ScreenWidget *ScreenWidget::Instance()
{
    if (self.isNull()) {
        static QMutex mutex;
        QMutexLocker locker(&mutex);
        if (self.isNull()) {
            self.reset(new ScreenWidget);
        }
    }

    return self.data();

}

ScreenWidget::ScreenWidget(QWidget *parent)
    : QWidget{parent}
{
    menu = new QMenu(this);
    menu->addAction("save", this, SLOT(saveScreen()));
    menu->addAction("save fullscreen", this, SLOT(saveFullScreen()));
    menu->addAction("save to", this, SLOT(saveScreenOther()));
    menu->addAction("save fullscreen to", this, SLOT(saveFullOther()));
    menu->addAction("exit", this, SLOT(hide()));
    //保存全屏图像
    fullScreen = new QPixmap();
}

void ScreenWidget::saveScreen()
{
    int x = fmin(startPos.x(),endPos.x());
    int y = fmin(startPos.y(),endPos.y());
    int x1 = fmax(startPos.x(),endPos.x());
    int y1 = fmax(startPos.y(),endPos.y());

    QString fileName = QString("%1/screen_%2.png").arg(qApp->applicationDirPath()).arg(STRDATETIME);
    fullScreen->copy(x, y, x1-x, y1-y).save(fileName, "png");
    close();

}

void ScreenWidget::saveFullScreen()
{

    close();
}

void ScreenWidget::saveScreenOther()
{

}

void ScreenWidget::saveFullOther(){

}
void ScreenWidget::contextMenuEvent(QContextMenuEvent *e){
    this->setCursor(Qt::ArrowCursor);
    menu->exec(cursor().pos());
}
bool ScreenWidget::InRect(const QPoint &pos)
{
    int x = fmin(startPos.x(),endPos.x());
    int y = fmin(startPos.y(),endPos.y());
    int x1 = fmax(startPos.x(),endPos.x());
    int y1 = fmax(startPos.y(),endPos.y());
    if(pos.x()>x&&pos.x()<x1&&pos.y()>y&&pos.y()<y1)
        return true;
    return false;
}

bool ScreenWidget::Move(const QPoint &delta)
{
    int w=qApp->primaryScreen()->geometry().width();
    int h=qApp->primaryScreen()->geometry().height();
    QPoint s=startPos+delta;
    QPoint e=endPos+delta;
    bool flag=false;
    if(s.x()>=0&&s.x()<w&&e.x()>=0&&e.x()<w){
        startPos.setX(s.x());
        endPos.setX(e.x());
        movPos+=QPoint(delta.x(),0);
        flag=true;
    }

    if(s.y()>=0&&s.y()<h&&e.y()>=0&&e.y()<h){
        startPos.setY(s.y());
        endPos.setY(e.y());
        movPos+=QPoint(0,delta.y());
        flag=true;
    }
    return flag;
}

void ScreenWidget::mousePressEvent(QMouseEvent *e){
    mousedown=true;
   if(InRect(e->pos())){
       inRect=true;
       movPos=e->pos();
   }
   else{
       startPos=e->pos();
       endPos=startPos;
       inRect=false;
   }
    this->update();
}
void ScreenWidget::mouseMoveEvent(QMouseEvent *e){
    if(mousedown){
       if(inRect){
           QPoint curPos=e->pos();
           QPoint Delta=curPos-movPos;
           Move(Delta);

       }
       else{
        endPos=e->pos();
       }
    }
    this->update();
}
void ScreenWidget::mouseReleaseEvent(QMouseEvent *e){
    mousedown=false;
    this->update();
}
void ScreenWidget::paintEvent(QPaintEvent *e){
    int x = fmin(startPos.x(),endPos.x());
    int y = fmin(startPos.y(),endPos.y());
    int x1 = fmax(startPos.x(),endPos.x());
    int y1 = fmax(startPos.y(),endPos.y());
    int w = x1-x;
    int h = y1-y;

    QPainter painter(this);
    QPen pen;
    pen.setColor(Qt::green);
    pen.setWidth(2);
    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);
    painter.drawPixmap(0, 0, *bgScreen);

    if (w != 0 && h != 0) {
        painter.drawPixmap(x, y, fullScreen->copy(x, y, w, h));
    }

    painter.drawRect(x, y, w, h);

    pen.setColor(Qt::yellow);
    painter.setPen(pen);
    painter.drawText(x + 2, y - 8, tr("截图范围：( %1 x %2 ) - ( %3 x %4 )  图片大小：( %5 x %6 )")
                     .arg(x).arg(y).arg(x + w).arg(y + h).arg(w).arg(h));
}
void ScreenWidget::showEvent(QShowEvent *e){
    QScreen *pscreen = QApplication::primaryScreen();
    int w=qApp->primaryScreen()->geometry().width();
    int h=qApp->primaryScreen()->geometry().height();
    *fullScreen = pscreen->grabWindow(0, 0, 0, w, h);

    //设置透明度实现模糊背景
    QPixmap pix(w, h);
    pix.fill((QColor(160, 160, 160, 200)));
    bgScreen = new QPixmap(*fullScreen);
    QPainter p(bgScreen);
    p.drawPixmap(0, 0, pix);
}
