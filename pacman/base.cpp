#include "base.h"

const int BaseH::deltax[5] = {-1,1,0,0,0};
const int BaseH::deltay[5] = {0,0,-1,1,0};
int BaseH::Width,BaseH::Height,BaseH::ObjectWidth;
int BaseH::startX,BaseH::startY;
int BaseH::map[35][35];

Base::Base(QPixmap pixmap)
    : QGraphicsPixmapItem(pixmap)
{
}

void Base::set_nxtDir(BaseH::dirstate dir){
    nxtDir = dir;
}
