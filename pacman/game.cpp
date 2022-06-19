#include "game.h"
#include <QTimer>
#include <QDialog>
#include <QDebug>
#include <QThread>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <iostream>
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
    QPixmap gatepng(":/images/gate.png");//这个是不是变成全局变量更好一点
    qDebug() << "244" << endl;
    static int num_ghost = 0;
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
                    break;
                case '1':
                    map[j][i] = Wall;
                    uimap[j][i] = new Base(wallpng);
                    break;
                case '2':
                    map[j][i] = Food;
                    uimap[j][i] = new Base(foodpng);
                    break;
                case '3':
                    map[j][i] = Medicine;
                    uimap[j][i] = new Base(medicinepng);
                    break;
                case '4':
                    map[j][i] = Door;
                    uimap[j][i] = new Base(gatepng);
                    doorx=j,doory=i;
                    break;
                case 'p':
                    pacman = new Pacman(j,i,this);
                    addItem(pacman);

                    map[j][i] = Space;
                    uimap[j][i] = new Base(blankpng);
                    break;
                case 'g':
                    ghost[num_ghost] = new Ghost(num_ghost,j,i,this);
                    addItem(ghost[num_ghost]);
                    num_ghost++;

                    map[j][i] = Space;
                    uimap[j][i] = new Base(blankpng);
                    break;

            }
            uimap[j][i]->setPos(x,y);
            addItem(uimap[j][i]);
        }
    }


    //pacman = new Pacman(1,1);//设置初始位置
    //addItem(pacman);

    qDebug() << "234 " << endl;
    for(int i=0;i<4;++i){
        ghost[i] -> get_dis_map();
        ghost[i] -> outcave_time = 200 + i * 800;
        ghost_timer[i] = new QTimer(this);
        connect(ghost_timer[i], &QTimer::timeout, [=](){this -> ghost_handler(i);});
    }
    pacman_timer = new QTimer(this);
    panic_timer = new QTimer(this);
    connect(pacman_timer, &QTimer::timeout, [=](){this -> pacman_handler();});

    this -> start();

}

void Game::start(){
    pacman_timer->start(INTERVAL_pacman);
    for(int i=0;i<4;++i){
        ghost_timer[i]->start(INTERVAL_ghost);
        ghost[i] -> state = Incave;
        ghost[i] -> set_curDir(Stop);
        ghost[i] -> set_nxtDir(Stop);
    }
}

void Game::over(){
    pacman_timer->stop();
    for(int i=0;i<4;++i) ghost_timer[i]->stop();
    panic_timer -> stop();
}


void Game::obtain(int x,int y){
    if(map[x][y] == Food){
        map[x][y] = Space;

        QPixmap blankpng;
        uimap[x][y] ->setPixmap(blankpng);
    }

    if(map[x][y] == Medicine){
        map[x][y] = Space;
        QPixmap blankpng;
        uimap[x][y] ->setPixmap(blankpng);

        QPixmap normalpng(":/images/pacman/a1.png");
        QPixmap panicpng(":/images/pacman/b1.png");
        pacman -> setPixmap(panicpng);
        pacman -> state = Panic;
        qDebug() << "START PANIC" << endl;
        panic_timer -> start(9000);
        connect(panic_timer, &QTimer::timeout, [=](){
            pacman -> state = Normal;
            panic_timer -> stop();
            pacman -> setPixmap(normalpng);
            qDebug() << "END PANIC" << endl;
        });
    }
}

void Game::ghost_handler(int p){
//    qDebug() << p << " " << ghost[p]->outcave_time << " " << ghost[p] ->state << endl;
    if(ghost[p]->outcave_time > 0){
        ghost[p] -> outcave_time -= INTERVAL_ghost;
        if(ghost[p] -> outcave_time <= 0){
            ghost[p] -> outcave_time = 0;
            ghost[p] -> state = Outingcave;
            //qDebug() << p << " -- " << ghost[p]->outcave_time << " " << ghost[p] ->state << endl;
        }
        return ;
    }
    //qDebug() << ghost[p] -> outcave_time << endl;
    ghost[p] -> move();

}

void Game::pacman_handler(){
    pacman -> move();
}

void Game::newpress(Qt::Key key){
    //qDebug() << "Press" << key  << endl;
    switch (key)
    {
    case Qt::Key_W : pacman -> set_nxtDir(Up); break;
    case Qt::Key_S : pacman -> set_nxtDir(Down); break;
    case Qt::Key_A : pacman -> set_nxtDir(Left); break;
    case Qt::Key_D : pacman -> set_nxtDir(Right); break;
    case Qt::Key_Up : pacman -> set_nxtDir(Up); break;
    case Qt::Key_Down : pacman -> set_nxtDir(Down); break;
    case Qt::Key_Left : pacman -> set_nxtDir(Left); break;
    case Qt::Key_Right : pacman -> set_nxtDir(Right); break;
        default: break;
    }
    //qDebug() << pacman_timer -> timeout() << endl;
}
Game::~Game()
{
    //Do Nothing
}
