#include "game.h"
#include <QDebug>
#include <Qpainter>

using namespace BaseH;
#define W ObjectWidth
#define IMAGE_INIT QPixmap("tmp.png").scaled(W,W)

Pacman::~Pacman(){
    //do nothing
}

Pacman::Pacman(int sx = 5,int sy = 5) : Base(IMAGE_INIT)
{
    nxtDir = Left;
    this -> setPos(startX + W*sx,startY+W*sy);
}

mapstate nxt(int x,int y,dirstate dir){
    x += deltax[dir];
    y += deltay[dir];
    if(x < 0 || x >= Width || y < 0 || y > Height) return Wall;
    return static_cast<mapstate> (map[x][y]);
}

void Pacman::move(){
//    preX,preY;
    int ox,oy;
    ox = this -> x() - startX;
    oy = this -> y() - startY;//ox,oy表示图片的地址和地图左上角的偏移量
    if(ox % W == 0 && oy % W == 0){
        preX = ox / W;
        preY = oy / W;
        curDir = nxtDir;
        if(nxt(preX,preY,curDir) == Wall) curDir = nxtDir = Stop;
    }
    this -> setPos(x()+deltax[curDir],y()+deltay[curDir]);
    qDebug() << x() << " " << y() << endl;
}
