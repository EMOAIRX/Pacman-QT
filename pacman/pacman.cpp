#include "game.h"
#include <QDebug>
#include <Qpainter>
#include <QGraphicsItem>

using namespace BaseH;
#define W ObjectWidth
#define IMAGE_INIT QPixmap(":/images/pacman/a1.png")

Pacman::~Pacman(){
    //do nothing
}

Pacman::Pacman(int sx,int sy,Game* father) : Base(IMAGE_INIT)
{
    init_posx = sx;
    init_posy = sy;
    game = father;
    nxtDir = Left;
    state = Normal;
    remain_panic_time = 0;
    animations[Left].push_back(QPixmap(":/images/pacman/pacmanleft1.png"));
    animations[Left].push_back(QPixmap(":/images/pacman/pacmanleft2.png"));
    animations[Left].push_back(QPixmap(":/images/pacman/pacmanleft3.png"));
    animations[Right].push_back(QPixmap(":/images/pacman/pacmanright1.png"));
    animations[Right].push_back(QPixmap(":/images/pacman/pacmanright2.png"));
    animations[Right].push_back(QPixmap(":/images/pacman/pacmanright3.png"));
    animations[Up].push_back(QPixmap(":/images/pacman/pacmanup1.png"));
    animations[Up].push_back(QPixmap(":/images/pacman/pacmanup2.png"));
    animations[Up].push_back(QPixmap(":/images/pacman/pacmanup3.png"));
    animations[Down].push_back(QPixmap(":/images/pacman/pacmandown1.png"));
    animations[Down].push_back(QPixmap(":/images/pacman/pacmandown2.png"));
    animations[Down].push_back(QPixmap(":/images/pacman/pacmandown3.png"));
    animations[Stop] = animations[Right];

    this -> setPos(startX + W*sx,startY+W*sy);
}

void Pacman::obtain(int x,int y){
    this -> game -> obtain(x,y);
}

void Pacman::caught(){
    game -> over();
}

pacmanstate Pacman::get_state() const{
    return state;
}

void Pacman::Collision_determination(){
    for(int i=0;i<4;++i){
        if(this -> QGraphicsItem::collidesWithItem(game->ghost[i],Qt::IntersectsItemShape)){
            if (game->ghost[i]->get_state()!=Outcave) continue;
            if(this -> state == Normal) this -> caught();
            if(this -> state == Panic)
            {
                game -> ghost[i] -> caught();
                game -> Score += game -> ghost[i] -> score;
            }
        }
    }
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


        if(preX == game -> Lposx && preY == game -> Lposy && curDir != Right)
            this -> setPos(startX + W*game->Rposx,startY+W*game->Rposy);
        if(preX == game -> Rposx && preY == game -> Rposy && curDir != Left)
            this -> setPos(startX + W*game->Lposx,startY+W*game->Lposy);
    }
    //会不会有bug啊，就是在中间的时候暂停一下——
//    if(preX==game->Lposx&&preY==game->Lposy)
//       qDebug() << "???" << curDir << endl;
    this -> setPos(x()+deltax[curDir],y()+deltay[curDir]);
    this -> Collision_determination();
//    qDebug() << this -> state << endl;
//    qDebug() << x() << " " << y() << endl;
}
