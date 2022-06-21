#include "base.h"

const int BaseH::deltax[5] = {-1,1,0,0,0};
const int BaseH::deltay[5] = {0,0,-1,1,0};
int BaseH::Width,BaseH::Height,BaseH::ObjectWidth;
int BaseH::startX,BaseH::startY;
int BaseH::map[35][35];
int BaseH::doorx,BaseH::doory;
const int BaseH::INTERVAL_ghost=10;
const int BaseH::INTERVAL_pacman=9;
const int BaseH::INTERVAL_flash = 250;
const int BaseH::score_food = 10;
const int BaseH::score_medicine = 20;

using namespace BaseH;

Base::Base(QPixmap pixmap,int Score_)
    : QGraphicsPixmapItem(pixmap)
{
    score = Score_;
}
Base::Base(QPixmap pixmap)
    : QGraphicsPixmapItem(pixmap)
{
}
Base::Base()
    : QGraphicsPixmapItem()
{
}

void Base::set_nxtDir(dirstate dir){
    nxtDir = dir;
}
void Base::set_curDir(dirstate dir){
    curDir = dir;
}

int Base::get_x() const{
    return preX;
}
int Base::get_y() const{
    return preY;
}

dirstate Base::get_curDir(){
    return curDir;
}

mapstate Base::nxt(int x,int y,dirstate dir){
    x += deltax[dir];
    y += deltay[dir];
    if(x < 0 || x >= Width || y < 0 || y >= Height) return Wall;
    return static_cast<mapstate> (map[x][y]);
}
mapstate Base::nxt(int x,int y,int dir){
    x += deltax[dir];
    y += deltay[dir];
    if(x < 0 || x >= Width || y < 0 || y >= Height) return Wall;
    return static_cast<mapstate> (map[x][y]);
}


bool Base::canmove(int x,int y,dirstate dir){
    auto val = nxt(x,y,dir);
    return val != Wall && val != Door;
}
