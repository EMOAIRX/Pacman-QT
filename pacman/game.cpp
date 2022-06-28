#include "game.h"
#include <QTimer>
#include <QDialog>
#include <QDebug>
#include <QThread>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <iostream>
#include <QList>
#include <QFile>
#include <mainwindow.h>
using namespace BaseH;

#define INTERVAL_PANIC_FLASH 20

Game::Game(int WW,int HH,QString map_src,MainWindow* fa) : QGraphicsScene(50,50,WW*32,HH*32)
{
    mainwindow = fa;
    Score = 0;
    died = 0;
    startX = 50;
    startY = 50;
    ObjectWidth = 32;
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
    QPixmap wallpng(":/images/map_objects/wall.png");
    QPixmap foodpng(":/images/map_objects/dot.png");
    QPixmap medicinepng(":/images/map_objects/power_ball.png");
    QPixmap gatepng(":/images/map_objects/gate.png");//这个是不是变成全局变量更好一点
    remain_food = 0;
    remain_medicine = 0;
    qDebug() << "244" << endl;
    int num_ghost = 0;
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
                    remain_food += 1;
                    break;
                case '3':
                    map[j][i] = Medicine;
                    uimap[j][i] = new Base(medicinepng,score_medicine);
                    powerball.push_back(uimap[j][i]);
                    remain_medicine += 1;
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
                    //ghost[num_ghost] = new Ghost(num_ghost,j,i,num_ghost+1,this);
                    //addItem(ghost[num_ghost]);
                    num_ghost++;

                    map[j][i] = Space;
                    uimap[j][i] = new Base(blankpng);
                    break;
                case 'L':
                    this -> Lposx = j;
                    this -> Lposy = i;
                    map[j][i] = Portal;
                    uimap[j][i] = new Base(blankpng,0);
                    break;
                case 'R':
                    this -> Rposx = j;
                    this -> Rposy = i;
                    map[j][i] = Portal;
                    uimap[j][i] = new Base(blankpng,0);
                    break;
            }
            uimap[j][i]->setPos(x,y);
            addItem(uimap[j][i]);
            qDebug() << i << " " << j << endl;
        }
    }
    for (int i = 0; i < num_ghost; i++) addItem(ghost[i]);
    qDebug() << "234 " << endl;
    for(int i=0;i<4;++i){
        ghost[i] -> get_dis_map();
        ghost_timer[i] = new QTimer(this);
        connect(ghost_timer[i], &QTimer::timeout, [=](){this -> ghost_handler(i);});
        ghost_animation_timer[i] = new QTimer(this);
        connect(ghost_animation_timer[i], &QTimer::timeout, [=](){this -> ghost_animation_handler(i);});
    }
    pacman_timer = new QTimer(this);
    pacman_animation_timer = new QTimer(this);
    panic_timer = new QTimer(this);
    panic_flash_tick = 1;
    connect(pacman_timer, &QTimer::timeout, [=](){this -> pacman_handler();});
    connect(pacman_animation_timer, &QTimer::timeout, [=](){this -> pacman_animation_handler();});
    powerball_flash_timer = new QTimer(this);
    connect(powerball_flash_timer, &QTimer::timeout, [=](){this -> powerball_flash();});
    connect(panic_timer, &QTimer::timeout, [=](){this -> panic_handler();});
    powerball_flash_timer->start(INTERVAL_flash);
    //ghost[0] -> setPixmap(orange_ghost);
    ghost[0] -> strategy = chasing_red(ghost[0],pacman);
    ghost[1] -> strategy = chasing_pink(ghost[1],pacman);
    ghost[2] -> strategy = chasing_orange(ghost[2],ghost[0],pacman);
    ghost[3] -> strategy = chasing_blue(ghost[3],pacman);

    this -> start();
}

void Game::replay(int WW,int HH,QString map_src)
{
    //clear();
    Score = 0;
    died = 0;
    startX = 50;
    startY = 50;
    ObjectWidth = 32;
    Width = WW;//X的范围
    Height = HH;//Y的范围
    qDebug() << "666 " << endl;
    QFile mapfile(map_src);
    powerball.clear();
    mapfile.open(QIODevice::ReadOnly|QIODevice::Text);
    QPixmap blankpng;
    QPixmap wallpng(":/images/map_objects/wall.png");
    QPixmap foodpng(":/images/map_objects/dot.png");
    QPixmap medicinepng(":/images/map_objects/power_ball.png");
    QPixmap gatepng(":/images/map_objects/gate.png");//这个是不是变成全局变量更好一点
    remain_food = 0;
    remain_medicine = 0;
    qDebug() << "244" << endl;
    int num_ghost = 0;
    for (int i=0;i<Height;i++)
    {
        QByteArray line = mapfile.readLine();
        for (int j=0;j<Width;j++)
        {
            int x = startX+j*ObjectWidth;
            int y = startY+i*ObjectWidth;
            qDebug() << i << " " << j << " " << x << " " << " " << y;
            qDebug() << line[j];
            qDebug() << uimap[j][i];

            switch (line[j]) {
                case '0':
                    map[j][i] = Space;
                    uimap[j][i] -> setPixmap(blankpng);
                    break;
                case '1':
                    map[j][i] = Wall;
                    uimap[j][i] -> setPixmap(wallpng);
                    break;
                case '2':
                    map[j][i] = Food;
                    uimap[j][i] -> setPixmap(foodpng);
                    remain_food += 1;
                    break;
                case '3':
                    map[j][i] = Medicine;
                    uimap[j][i] -> setPixmap(medicinepng);
                    powerball.push_back(uimap[j][i]);
                    remain_medicine += 1;
                    break;
                case '4':
                    map[j][i] = Door;
                    uimap[j][i] -> setPixmap(gatepng);
                    doorx=j,doory=i;
                    break;
                case 'p':
                    pacman->setPos(x,y);

                    map[j][i] = Space;
                    uimap[j][i] = new Base(blankpng);
                    break;
                case 'g':
                    ghost[num_ghost] ->setPos(x,y);
                    num_ghost++;

                    map[j][i] = Space;
                    uimap[j][i] -> setPixmap(blankpng);
                    break;
                case 'L':
                    this -> Lposx = j;
                    this -> Lposy = i;
                    map[j][i] = Portal;
                    uimap[j][i] -> setPixmap(blankpng);
                    break;
                case 'R':
                    this -> Rposx = j;
                    this -> Rposy = i;
                    map[j][i] = Portal;
                    uimap[j][i] -> setPixmap(blankpng);
                    break;
            }
            uimap[j][i]->setPos(x,y);
        }
    }
    qDebug() << "234 " << endl;
    for(int i=0;i<4;++i){
        ghost[i] -> get_dis_map();
        delete ghost_timer[i];
        ghost_timer[i] = new QTimer(this);
        connect(ghost_timer[i], &QTimer::timeout, [=](){this -> ghost_handler(i);});
    }
    delete pacman_timer;
    pacman_timer = new QTimer(this);
    delete pacman_animation_timer;
    pacman_animation_timer = new QTimer(this);
    delete panic_timer;
    panic_timer = new QTimer(this);
    panic_flash_tick = 1;
    connect(pacman_timer, &QTimer::timeout, [=](){this -> pacman_handler();});
    connect(pacman_animation_timer, &QTimer::timeout, [=](){this -> pacman_animation_handler();});
    powerball_flash_timer = new QTimer(this);
    connect(powerball_flash_timer, &QTimer::timeout, [=](){this -> powerball_flash();});
    connect(panic_timer, &QTimer::timeout, [=](){this -> panic_handler();});
    powerball_flash_timer->start(INTERVAL_flash);
    //ghost[0] -> setPixmap(orange_ghost);
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
    qDebug()<<"start"<<endl;
    pacman->remain_panic_flash_time=0;
    pacman->remain_panic_time=0;
    pacman->reposition();//(startX+W*pacman->init_posx,startY+pacman->init_posy);
    pacman->set_curDir(Left);
    pacman->set_nxtDir(Left);
    stat = Start;
    panic_timer -> start(INTERVAL_PANIC_FLASH);
    pacman_timer-> start(INTERVAL_pacman);
    pacman_animation_timer -> start(INTERVAL_pacman_animation);
    for(int i=0;i<4;++i){
        ghost_timer[i]->start(INTERVAL_ghost);
        ghost_animation_timer[i]->start(INTERVAL_ghost_animation);
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
    died++;
  //  stat = Over;
    if (died < 3)
    {
        start();
        Score -= 800;
    }
    else
    {
        lose();
        return;
    }
}

void Game::win(){
    Score += remain_medicine * 500 + 1000;
    pause();
    stat = Over;
}

void Game::lose()
{
    pause();
    stat = Over;
}

void Game::pause(){
    if(stat == Over) return;
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
        remain_food -= 1;
    }

    if(map[x][y] == Medicine){
        map[x][y] = Space;
        Score+=uimap[x][y]->score;
        remain_medicine -= 1;
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

        pacman->remain_panic_time=5000;
        pacman->remain_panic_flash_time=2000;
    }
    if(remain_food == 0){
        win();
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


void Game::pacman_animation_handler() {
    if (pacman->state != Panic) {
        pacman->animation_index++;
        if (pacman->animation_index >= pacman->animations[pacman->curDir].size()) pacman->animation_index = 0;
        pacman->setPixmap(pacman->animations[pacman->curDir][pacman->animation_index]);
    }
}

void Game::ghost_animation_handler(int i) {
    if (ghost[i]->state != Backingcave) {
        ghost[i]->animation_index++;
        if (ghost[i]->animation_index >= ghost[i]->animations[ghost[i]->curDir].size()) ghost[i]->animation_index = 0;
        ghost[i] -> setPixmap(ghost[i]->animations[ghost[i]->curDir][ghost[i]->animation_index]);
    }
}

void Game::newpress(Qt::Key key){
    //qDebug() << "Press" << key  << endl;
    //qDebug() << Qt::Key_Enter << endl;
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
    case Qt::Key_Return : if (this -> stat == Over){
            int w = 27, h = 21;
            mainwindow -> lose_label->hide();
            mainwindow -> win_label -> hide();
            mainwindow -> playagain -> hide();
            mainwindow -> score_timer -> start(25);
            this -> replay(w,h,":/map/map1.txt");
        } break;
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
    //qDebug()<<"hhh "<<flash_tick<<endl;
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
    for (int i = 0; i < Height; i++) {
        for (int j = 0; j < Width; j++) {
            if (uimap[i][j] != nullptr)
                delete uimap[i][j];
        }
        delete[] uimap[i];
    }
    delete[] uimap;
    delete pacman_timer;
    delete pacman_animation_timer;
    delete powerball_flash_timer;
    for (int i=0;i<4;i++) delete ghost[i];
}
