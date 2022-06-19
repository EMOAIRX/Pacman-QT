#include "game.h"
#include <QDebug>
#include <Qpainter>

using namespace BaseH;
#define W ObjectWidth
#define IMAGE_INIT QPixmap(":/images/pacman/a1.png")

Pacman::~Pacman(){
    //do nothing
}

Pacman::Pacman(int sx,int sy,Game* father) : Base(IMAGE_INIT)
{
    game = father;
    nxtDir = Left;
    this -> setPos(startX + W*sx,startY+W*sy);
}

mapstate nxt(int x,int y,dirstate dir){
    x += deltax[dir];
    y += deltay[dir];
    if(x < 0 || x >= Width || y < 0 || y > Height) return Wall;
    return static_cast<mapstate> (map[x][y]);
}

bool canmove(int x,int y,dirstate dir){
    auto val = nxt(x,y,dir);
    return val != Wall && val != Door;
}

void Pacman::obtain(int x,int y){
    this -> game -> obtain(x,y);
}


void Pacman::move(){
//    preX,preY;
    int ox,oy;
    ox = this -> x() - startX;
    oy = this -> y() - startY;//ox,oy表示图片的地址和地图左上角的偏移量
    if(ox % W == 0 && oy % W == 0){
        preX = ox / W;
        preY = oy / W;
        this -> obtain(preX,preY);
        if(canmove(preX,preY,nxtDir)) curDir = nxtDir;
        if(!canmove(preX,preY,curDir)) curDir = Stop;
        nxtDir = curDir;//这里手感好像不太好，写完可以微调一下
    }
    this -> setPos(x()+deltax[curDir],y()+deltay[curDir]);
    qDebug() << x() << " " << y() << endl;
}
