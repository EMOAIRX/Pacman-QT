#include "game.h"
#include <QTimer>
#include <QDialog>
#include <QDebug>
#include <QThread>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>

using namespace BaseH;

Gamee::Gamee(QString xxxx) : QGraphicsScene(80,100,400,400)
{
    startX = 80;
    startY = 100;
    ObjectWidth = 40;
    Width = 20;//X的范围
    Height = 20;//Y的范围
//    qDebug() << "666 " << endl;
    for(int i=0;i<Width;++i)
        for(int j=0;j<Height;++j)
            map[i][j] = Space;
//    qDebug() << "555 " << endl;
    pacman = new Pacman(1,1);//设置初始位置
    addItem(pacman);

    qDebug() << "234 " << endl;
    pacman_timer = new QTimer(this);
    pacman_timer->start(10);
    timerThread = new QThread;
    pacman_timer -> moveToThread(timerThread);
    connect(pacman_timer, &QTimer::timeout, [=](){this -> pacman_handler();});
    timerThread -> start();

}

void Gamee::pacman_handler(){
    pacman -> move();
//    qDebug() << x() << " " << y() << endl;

//    qDebug() << "what fuck " << endl;
}

void Gamee::newpress(Qt::Key key){
    qDebug() << "Press" << key  << endl;
    switch (key)
    {
        case Qt::Key_W : pacman -> set_nxtDir(Up); break;
        case Qt::Key_S : pacman -> set_nxtDir(Down); break;
        case Qt::Key_A : pacman -> set_nxtDir(Left); break;
        case Qt::Key_D : pacman -> set_nxtDir(Right); break;
        default: break;
    }
    //qDebug() << pacman_timer -> timeout() << endl;
}
Gamee::~Gamee()
{
    //Do Nothing
}
