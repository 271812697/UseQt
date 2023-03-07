#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QPainter>
#include<QDebug>
#include<QFont>
#include<QTimer>
static int p[20][4][4]={
    {0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},
    {0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},
    {0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},
    {0,0,0,0,0,1,1,0,0,1,1,0,0,0,0,0},
    {0,1,0,0,
     0,1,0,0,
     0,1,0,0,
     0,1,0,0},
    {0,0,0,0,
     1,1,1,1,
     0,0,0,0,
     0,0,0,0},
    {0,1,0,0,
     0,1,0,0,
     0,1,0,0,
     0,1,0,0},
    {0,0,0,0,
     1,1,1,1,
     0,0,0,0,
     0,0,0,0},

    {0,0,0,0,
     0,1,0,0,
     1,1,1,0,
     0,0,0,0},
    {0,1,0,0,
     0,1,1,0,
     0,1,0,0,
     0,0,0,0},
    {0,0,0,0,
     1,1,1,0,
     0,1,0,0,
     0,0,0,0},
    {0,1,0,0,
     1,1,0,0,
     0,1,0,0,
     0,0,0,0},

    {0,0,0,0,
     0,1,1,0,
     0,1,0,0,
     0,1,0,0},
    {0,0,0,0,
     1,1,1,0,
     0,0,1,0,
     0,0,0,0},
    {0,1,0,0,
     0,1,0,0,
     1,1,0,0,
     0,0,0,0},
    {0,1,0,0,
     0,1,1,1,
     0,0,0,0,
     0,0,0,0},


    {0,0,0,0,
     1,1,0,0,
     0,1,1,0,
     0,0,0,0},
    {0,0,1,0,
     0,1,1,0,
     0,1,0,0,
     0,0,0,0},
    {0,0,0,0,
     1,1,0,0,
     0,1,1,0,
     0,0,0,0},
    {0,0,1,0,
     0,1,1,0,
     0,1,0,0,
     0,0,0,0}
};
static int table[16][12];
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    cur=19;
    ui->setupUi(this);
    base1.rx()=600;
    base1.ry()=0;
    next=rand()%20;
    cur=rand()%20;
    base.rx()=4*50;
    base.ry()=0;

    F=QFont("Times", 20, QFont::Bold);
    this->setGeometry(0,0,800,800);
    T=new QTimer(this);
    connect(T, SIGNAL(timeout()), this, SLOT(drop()));
    T->start(500);
}
void MainWindow::erase(){
    for(int i=15;i>=0;i--){
        int sum=0;
        for(int j=0;j<12;j++){
            if(table[i][j]==1)sum++;
        }
        if(sum==12){
            score+=10;
            for(int k=0;k<12;k++){
                for(int x=i;x>0;x--){
                    table[x][k]=table[x-1][k];
                }
            }

        }
    }

}
void MainWindow::init(){
    for(int i=0;i<16;i++){
        for(int j=0;j<12;j++){
            table[i][j]=0;
        }
    }

}
void MainWindow::drop(){
    move(4);
}
bool MainWindow::is(QPoint base){
    int col=base.rx()/50;
    int row=base.ry()/50;
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++){
           if(p[cur][i][j]==1){
               if((row+i)>=0&&(row+i)<16&&(col+j)>=0&&(col+j)<12){
                   if(table[row+i][col+j]==1)return  false;
               }
               else if((row+i)<0||(row+i)>=16||(col+j)<0||(col+j)>=12){
                   return false;
               }
           }
        }
    }
    return true;
}
void MainWindow::move(int a){
    switch (a) {
    case 1:{//左移动
            if(is(QPoint(base.rx()-50,base.ry()))){
                base.rx()-=50;
            }
    }break;
    case 2:{//右边移动
        if(is(QPoint(base.rx()+50,base.ry()))){
            base.rx()+=50;
        }
    }break;
    case 3:{//上移动
              int z=cur;
              int t=cur%4;
              cur-=t;
              t=(t+1)%4;
              cur+=t;
              if(!is(base)){
               cur=z;
              }

    }break;
    case 4:{//下移动
        if(is(QPoint(base.rx(),base.ry()+50))){
            base.ry()+=50;
        }
        else{
            int col=base.rx()/50;
            int row=base.ry()/50;
            for(int i=0;i<4;i++){
                for(int j=0;j<4;j++){
                    if(p[cur][i][j]==1){
                        table[row+i][col+j]=1;
                    }
                }
            }
            erase();
            cur=next;
            next=rand()%20;
            base.rx()=4*50;
            base.ry()=0;
            if(!is(base)){
                init();
            }

        }
    }break;
    case 5:{
        while(is(QPoint(base.rx(),base.ry()+50))){
            base.ry()+=50;
        }
        int col=base.rx()/50;
        int row=base.ry()/50;
        for(int i=0;i<4;i++){
            for(int j=0;j<4;j++){
                if(p[cur][i][j]==1){
                    table[row+i][col+j]=1;
                }
            }
        }
        erase();
        cur=next;
        next=rand()%20;
        base.rx()=4*50;
        base.ry()=0;
        if(!is(base)){
            init();
        }

    }break;
    }
    this->update();
}
void MainWindow::keyPressEvent(QKeyEvent *event){
   if(event->key()==Qt::Key::Key_Left||event->key()==Qt::Key::Key_A){
      move(1);
   }
   if(event->key()==Qt::Key::Key_Right||event->key()==Qt::Key::Key_D){
      move(2);
   }
   if(event->key()==Qt::Key::Key_Up||event->key()==Qt::Key::Key_W){
      move(3);
   }
   if(event->key()==Qt::Key::Key_Down||event->key()==Qt::Key::Key_S){
      move(4);
   }
   if(event->key()==Qt::Key::Key_Space){
      move(5);
   }
}
void MainWindow::temp(QPainter*painter,QPoint base,int index){
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            if(p[index][i][j]==1)painter->drawRect(base.rx()+j*50,base.ry()+i*50,50,50);
        }
    }
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            if(p[next][i][j]==1)painter->drawRect(base1.rx()+j*50,base1.ry()+i*50,50,50);
        }
    }
    for(int i=0;i<16;i++){
        for(int j=0;j<12;j++){
            if(table[i][j]==1){
                painter->drawRect(j*50,i*50,50,50);
            }
        }
    }

   painter->setPen(QColor(100,0,0));
   painter->setFont(F);
   painter->drawText(650,220,QString("next block"));
   painter->drawText(600,500,"current score:"+QString::number(score));


}
void MainWindow::paintEvent(QPaintEvent *event){
    QPainter* paint=new QPainter(this);
    paint->fillRect(0,0,600,800,QGradient::NightFade);
    paint->fillRect(600,0,200,800,QGradient::Nega);
    paint->setRenderHint(QPainter::Antialiasing);
    paint->setPen(QColor(10,100,125,0.5));
    paint->setBrush(QColor(10,100,125));
    temp(paint,base,cur);
    paint->end();
}
MainWindow::~MainWindow()
{
    delete ui;
}

