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

#define INTERVAL_PANIC_FLASH 20

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
                    uimap[j][i] = new Base(blankpng,0);
                    break;
                case '1':
                    map[j][i] = Wall;
                    uimap[j][i] = new Base(wallpng,0);
                    break;
                case '2':
                    map[j][i] = Food;
                    uimap[j][i] = new Base(foodpng,score_food);
                    break;
                case '3':
                    map[j][i] = Medicine;
                    uimap[j][i] = new Base(medicinepng,score_medicine);
                    powerball.push_back(uimap[j][i]);
                    break;
                case '4':
                    map[j][i] = Door;
                    uimap[j][i] = new Base(gatepng,0);
                    doorx=j,doory=i;
                    break;
                case 'p':
                    pacman = new Pacman(j,i,this);
                    addItem(pacman);

                    map[j][i] = Space;
                    uimap[j][i] = new Base(blankpng);
                    break;
                case 'g':
                    ghost[num_ghost] = new Ghost(num_ghost,j,i,num_ghost+1,this);
                    //addItem(ghost[num_ghost]);
                    num_ghost++;

                    map[j][i] = Space;
                    uimap[j][i] = new Base(blankpng);
                    break;

            }
            uimap[j][i]->setPos(x,y);
            addItem(uimap[j][i]);
        }
    }
    for (int i = 0; i < num_ghost; i++)
        addItem(ghost[i]);

    qDebug() << "234 " << endl;
    for(int i=0;i<4;++i){
        ghost[i] -> get_dis_map();
        ghost_timer[i] = new QTimer(this);
        connect(ghost_timer[i], &QTimer::timeout, [=](){this -> ghost_handler(i);});
    }
    pacman_timer = new QTimer(this);
    panic_timer = new QTimer(this);
    panic_flash_tick = 1;
    connect(pacman_timer, &QTimer::timeout, [=](){this -> pacman_handler();});
    powerball_flash_timer = new QTimer(this);
    connect(powerball_flash_timer, &QTimer::timeout, [=](){this -> powerball_flash();});
    connect(panic_timer, &QTimer::timeout, [=](){this -> panic_handler();});
    powerball_flash_timer->start(INTERVAL_flash);
    ghost[0] -> strategy = chasing_red(ghost[0],pacman);
    ghost[1] -> strategy = chasing_pink(ghost[1],pacman);
    ghost[2] -> strategy = chasing_orange(ghost[2],ghost[0],pacman);
    ghost[3] -> strategy = chasing_blue(ghost[3],pacman);
    this -> start();
}
void Game::panic_handler(){
    int &rtime = pacman ->remain_panic_time;
    int &rrtime = pacman ->remain_panic_flash_time;
    if(rtime == 0) return ;
   // qDebug() << rtime << " " << rrtime << " ";
    rtime -= INTERVAL_PANIC_FLASH;
    QPixmap normalpng(":/images/pacman/a1.png");
    QPixmap panicpng(":/images/pacman/b1.png");
    if(rtime <= 0){
        rtime = 0;
        pacman -> state = Normal;
        pacman -> setPixmap(normalpng);
        pacman -> show();
        panic_flash_tick = 1;
        return ;
    }
    rrtime -= INTERVAL_PANIC_FLASH;
    if(rrtime <= 0){
        rrtime = std::max(10,rtime / 10);
        if(panic_flash_tick == 1)
            pacman -> setPixmap(normalpng),panic_flash_tick = 0;
        else
            pacman -> setPixmap(panicpng),panic_flash_tick = 1;
    }
}


void Game::start(){
    pacman->remain_panic_flash_time=0;
    pacman->remain_panic_time=0;
    pacman->reposition();//(startX+W*pacman->init_posx,startY+pacman->init_posy);
    pacman->set_curDir(Left);
    pacman->set_nxtDir(Left);
    stat = Start;
    panic_timer -> start(INTERVAL_PANIC_FLASH);
    pacman_timer-> start(INTERVAL_pacman);
    for(int i=0;i<4;++i){
        ghost_timer[i]->start(INTERVAL_ghost);
        ghost[i] -> outcave_time = 200 + i * 800;
        ghost[i] -> state = Incave;
        ghost[i] -> reposition();
        ghost[i] -> set_curDir(Stop);
        ghost[i] -> set_nxtDir(Stop);
    }
}

void Game::over(){
    //pacman_timer->stop();
    //for(int i=0;i<4;++i) ghost_timer[i]->stop();
    //panic_timer -> stop();
    //stat = Over;

    start();
    Score -= 800;
}


void Game::pause(){
    if(stat == Over) return ;
    if(stat == Start){
        stat = Pause;
        pacman_timer->stop();
        for(int i=0;i<4;++i) ghost_timer[i]->stop();
        panic_timer -> stop();
    } else{
        stat = Start;
        pacman_timer-> start(INTERVAL_pacman);
        for(int i=0;i<4;++i)
            ghost_timer[i]->start(INTERVAL_ghost);
        panic_timer->start(INTERVAL_PANIC_FLASH);
    }
}


void Game::obtain(int x,int y){
    if(map[x][y] == Food){
        map[x][y] = Space;
        QPixmap blankpng;
        uimap[x][y] ->setPixmap(blankpng);
        Score+=uimap[x][y]->score;
    }

    if(map[x][y] == Medicine){
        map[x][y] = Space;
        Score+=uimap[x][y]->score;
        QPixmap blankpng;
        for (int i = 0; i < this->powerball.size(); i++) {
            if (this->powerball.at(i) == uimap[x][y]) {
                this->powerball.remove(i);
                break;
            }
        }
        uimap[x][y] ->setPixmap(blankpng);
        QPixmap normalpng(":/images/pacman/a1.png");
        QPixmap panicpng(":/images/pacman/b1.png");
        pacman -> setPixmap(panicpng);
        pacman -> state = Panic;
        //qDebug() << "START PANIC" << endl;

        pacman->remain_panic_time=9000;
        pacman->remain_panic_flash_time=5000;
    }
}

void Game::ghost_handler(int p){
   // qDebug() << p << " " << ghost[p]->outcave_time << " " << ghost[p] ->state << endl;
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
    case Qt::Key_Space : this -> pause(); break;
        default: break;
    }
    //qDebug() << pacman_timer -> timeout() << endl;
}

void Game::powerball_flash()
{
    if (powerball.empty()) {
        powerball_flash_timer->stop();
        return;
    }
    qDebug()<<"hhh "<<flash_tick<<endl;
    if (flash_tick==2) {
        for (int i = 0; i < powerball.size(); i++) {
            powerball.at(i)->hide();
        }
        flash_tick = 0;
    } else {
        for (int i = 0; i < powerball.size(); i++) {
            powerball.at(i)->show();
        }
        flash_tick++;
    }
}

Game::~Game()
{
    //Do Nothing
}
