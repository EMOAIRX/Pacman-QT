#include "game.h"
#include <QTimer>
#include <QDialog>
#include <QDebug>
#include <QThread>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QFile>
using namespace BaseH;

Game::Game(int WW,int HH,QString map_src) : QGraphicsScene(50,50,WW*20,HH*20)
{
    startX = 50;
    startY = 50;
    ObjectWidth = 20;
    Width = WW;//X的范围
    Height = HH;//Y的范围
    qDebug() << "666 " << endl;
    uimap = new Base**[Width];
    for(int i=0;i<Width;++i)
    {
        uimap[i] = new Base*[Height];
        for(int j=0;j<Height;++j)
        {
            map[i][j] = Space;
            uimap[i][j] = nullptr;
        }
          //    qDebug() << "555 " << endl;
    }
    QFile mapfile(map_src);
    mapfile.open(QIODevice::ReadOnly|QIODevice::Text);
    QPixmap blankpng;
    QPixmap wallpng(":/images/wall.png");
    QPixmap foodpng(":/images/dot.png");
    QPixmap medicinepng(":/images/power_ball.png");
    QPixmap gatepng(":/images/gate.png");
    QPixmap ghostpng(":/images/ghost/bl1.png");
    qDebug() << "244" << endl;
    for (int i=0;i<Height;i++)
    {
        QByteArray line = mapfile.readLine();
        for (int j=0;j<Width;j++)
        {
            int x = startX+j*ObjectWidth;
            int y = startY+i*ObjectWidth;
            switch (line[j]) {
                case '0':
                    map[j][i] = Space;
                    uimap[j][i] = new Base(blankpng);
                    uimap[j][i]->setPos(x,y);
                    addItem(uimap[j][i]);
                    break;
                case '1':
                    map[j][i] = Wall;
                    uimap[j][i] = new Base(wallpng);
                    uimap[j][i]->setPos(x,y);
                    addItem(uimap[j][i]);
                    break;
                case '2':
                    map[j][i] = Food;
                    uimap[j][i] = new Base(foodpng);
                    uimap[j][i]->setPos(x,y);
                    addItem(uimap[j][i]);
                    break;
                case '3':
                    map[j][i] = Medicine;
                    uimap[j][i] = new Base(medicinepng);
                    uimap[j][i]->setPos(x,y);
                    addItem(uimap[j][i]);
                    break;
                case '4':
                    map[j][i] = Door;
                    uimap[j][i] = new Base(gatepng);
                    uimap[j][i]->setPos(x,y);
                    addItem(uimap[j][i]);
                    break;
                case 'p':
                    pacman = new Pacman(j,i);
                    map[j][i] = Space;
                    uimap[j][i] = pacman;
                    addItem(pacman);
                    break;
                case 'g':
                    map[j][i] = Space;
                    uimap[j][i] = new Base(ghostpng);
                    uimap[j][i]->setPos(x,y);
                    addItem(uimap[j][i]);
                    break;

            }
        }
    }
    //pacman = new Pacman(1,1);//设置初始位置
    //addItem(pacman);

    qDebug() << "234 " << endl;
    pacman_timer = new QTimer(this);
    pacman_timer->start(10);
    timerThread = new QThread;
    pacman_timer -> moveToThread(timerThread);
    connect(pacman_timer, &QTimer::timeout, [=](){this -> pacman_handler();});
    timerThread -> start();

}

void Game::pacman_handler(){
    pacman -> move();
//    qDebug() << x() << " " << y() << endl;

//    qDebug() << "what fuck " << endl;
}

void Game::newpress(Qt::Key key){
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
Game::~Game()
{
    //Do Nothing
}
