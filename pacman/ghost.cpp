#include "ghost.h"
#include <QDebug>
using namespace BaseH;
#define W ObjectWidth
#define IMAGE_INIT QPixmap(":/images/ghost/bl1.png")
//Ghost::Ghost(){

//}

Ghost::Ghost(int ghid,int sx,int sy,Game* father) : Base(IMAGE_INIT)
{
    init_posx = sx;
    init_posy = sy;
    id = ghid;
    game = father;
    this -> setPos(startX + W*sx,startY+W*sy);
    curDir = nxtDir = Stop;

}

void Ghost::caught(){
    this -> setPos(startX + W*init_posx,startY+W*init_posy);
    //这里需要回到笼子，具体怎么搞之后再说
}

bool Ghost::canmove(int x,int y,dirstate dir){
    auto val = Base::nxt(x,y,dir);
    if(val == Wall) return false;
    if(val == Door && outcave_time > 0) return false;
    return true;
}


void Ghost::move(){
    //preX,preY;
    int ox,oy;
    ox = this -> x() - startX;
    oy = this -> y() - startY;//ox,oy表示图片的地址和地图左上角的偏移量
    if(ox % W == 0 && oy % W == 0){
        preX = ox / W;
        preY = oy / W;
        //this -> obtain(preX,preY);
        curDir = static_cast<dirstate> (rand() % 4);
        while(!canmove(preX,preY,curDir)){
            curDir = static_cast<dirstate> (rand() % 4);
        }

    }
    this -> setPos(x()+deltax[curDir],y()+deltay[curDir]);
    qDebug() << x() << " " << y() << endl;
}//ghost的移动要专门根据策略设置过，这里不清楚怎么搞，或许可以讨论一下
