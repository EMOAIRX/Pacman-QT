#include "ghost.h"
#include <QDebug>
#include <iostream>
using namespace BaseH;
#define W ObjectWidth
#define IMAGE_INIT QPixmap(":/images/ghost/bl1.png")
#define run_image QPixmap(":/images/ghost/runleft.png")

#define orange_ghost QPixmap(":/images/ghost/orangeright1.png/");
#define red_ghost QPixmap(":/images/ghost/redright1.png");
#define blue_ghost QPixmap(":/images/ghost/blueright1.png");
#define pink_ghost QPixmap(":/images/ghost/pinkright1.png");
#define orangeleft QPixmap(":/images/ghost/orangeleft1.png");
#define redleft QPixmap(":/images/ghost/redleft1.png");
#define blueleft QPixmap(":/images/ghost/blueleft1.png");
#define pinkleft QPixmap(":/images/ghost/pinkleft1.png");
//Ghost::Ghost(){

//}

void Ghost::get_dis_map(){
#define fi first
#define se second

/*for(int j=0;j<Height;++j){
    for(int i=0;i<Width;++i){
        std::cout << " " <<map[i][j];
    }std::cout << "\n";
}*/
    typedef std::pair<int,int> pii;
    int front,rear;
    pii q[35*35];
    front=rear=0;
    memset(dis_map,-1,sizeof(dis_map));
    q[rear++]={init_posx,init_posy};
    dis_map[init_posx][init_posy] = 0;
    while(front<rear){
        pii u = q[front++];
        for(int d=0;d<4;++d){
            if(nxt(u.fi,u.se,d) == Wall) continue;
            pii v={u.fi+deltax[d],u.se+deltay[d]};
            if(dis_map[v.fi][v.se] == -1){
                dis_map[v.fi][v.se]=dis_map[u.fi][u.se]+1;
                q[rear++]=v;
            }
        }
    }
/*for(int j=0;j<Height;++j){
    for(int i=0;i<Width;++i){
        std::cout << " " <<map[i][j];
    }std::cout << "\n";
}*/
#undef fi
#undef se
}

Ghost::Ghost(int ghid,int sx,int sy,int numb,Game* father) : Base(IMAGE_INIT,numb*100)
{
    //img(IMAGE_INIT);
    if (numb == 1) {  // red
        animations[Left].push_back(QPixmap(":/images/ghost/redleft1.png"));
        animations[Left].push_back(QPixmap(":/images/ghost/redleft2.png"));
        animations[Right].push_back(QPixmap(":/images/ghost/redright1.png"));
        animations[Right].push_back(QPixmap(":/images/ghost/redright2.png"));
        animations[Up].push_back(QPixmap(":/images/ghost/redup1.png"));
        animations[Up].push_back(QPixmap(":/images/ghost/redup2.png"));
        animations[Down].push_back(QPixmap(":/images/ghost/reddown1.png"));
        animations[Down].push_back(QPixmap(":/images/ghost/reddown2.png"));
    }
    if (numb == 2) {  // pink
        animations[Left].push_back(QPixmap(":/images/ghost/pinkleft1.png"));
        animations[Left].push_back(QPixmap(":/images/ghost/pinkleft2.png"));
        animations[Right].push_back(QPixmap(":/images/ghost/pinkright1.png"));
        animations[Right].push_back(QPixmap(":/images/ghost/pinkright2.png"));
        animations[Up].push_back(QPixmap(":/images/ghost/pinkup1.png"));
        animations[Up].push_back(QPixmap(":/images/ghost/pinkup2.png"));
        animations[Down].push_back(QPixmap(":/images/ghost/pinkdown1.png"));
        animations[Down].push_back(QPixmap(":/images/ghost/pinkdown2.png"));
    }
    if (numb == 3) {  // orange
        animations[Left].push_back(QPixmap(":/images/ghost/orangeleft1.png"));
        animations[Left].push_back(QPixmap(":/images/ghost/orangeleft2.png"));
        animations[Right].push_back(QPixmap(":/images/ghost/orangeright1.png"));
        animations[Right].push_back(QPixmap(":/images/ghost/orangeright2.png"));
        animations[Up].push_back(QPixmap(":/images/ghost/orangeup1.png"));
        animations[Up].push_back(QPixmap(":/images/ghost/orangeup2.png"));
        animations[Down].push_back(QPixmap(":/images/ghost/orangedown1.png"));
        animations[Down].push_back(QPixmap(":/images/ghost/orangedown2.png"));
    }
    if (numb == 4) {  // blue
        animations[Left].push_back(QPixmap(":/images/ghost/blueleft1.png"));
        animations[Left].push_back(QPixmap(":/images/ghost/blueleft2.png"));
        animations[Right].push_back(QPixmap(":/images/ghost/blueright1.png"));
        animations[Right].push_back(QPixmap(":/images/ghost/blueright2.png"));
        animations[Up].push_back(QPixmap(":/images/ghost/blueup1.png"));
        animations[Up].push_back(QPixmap(":/images/ghost/blueup2.png"));
        animations[Down].push_back(QPixmap(":/images/ghost/bluedown1.png"));
        animations[Down].push_back(QPixmap(":/images/ghost/bluedown2.png"));
    }
    animations[Stop] = animations[Right];
    animation_index = 0;
    faceto = Right;
    setPixmap(animations[faceto][animation_index]);
    init_posx = sx;
    init_posy = sy;
    id = ghid;
    game = father;
    this -> setPos(startX + W*sx,startY+W*sy);
    curDir = nxtDir = Stop;
}
BaseH::ghoststate Ghost::get_state()
{
    return state;
}

void Ghost::caught(){
    if (state!=Outcave) return;
    state = Backingcave;
    this -> setPixmap(run_image);
    //this -> setPos(startX + W*init_posx,startY+W*init_posy);
    //??????????????????????????????????????????????????????
}

bool Ghost::canmove(int x,int y,dirstate dir){
    auto val = Base::nxt(x,y,dir);
    if (val == Wall) return false;
    if (val == Portal) return false;
    if (val == Door && state==Outcave) return false;
    if (val == Door && state==Incave && outcave_time>0) return false;
    return true;
}


void Ghost::move(){
    //preX,preY;
    int ox,oy;
    ox = this -> x() - startX;
    oy = this -> y() - startY;//ox,oy???????????????????????????????????????????????????
    if(ox % W == 0 && oy % W == 0){
        preX = ox / W;
        preY = oy / W;
        switch (state)
        {
            case Outingcave:
                //qDebug()<<"1234"<<endl;
                if (map[preX][preY] == Door)
                {
                    state = Outcave;
                    curDir = Up;
                }
                else
                {
                    for (int d=0;d<4;++d)
                    {
                        int nxtX=preX+deltax[d],nxtY=preY+deltay[d];
                        auto val=map[nxtX][nxtY];
                        if (val==Wall) continue;
                        if (val==Door)
                        {
                            curDir=static_cast<dirstate> (d);
                            break;
                        }
                        if (val==Space && abs(doorx-nxtX)+abs(doory-nxtY)<abs(doorx-preX)+abs(doory-preY))
                        {
                            curDir=static_cast<dirstate> (d);
                            break;
                        }
                    }
                }
                //qDebug() << id;
                //qDebug() << curDir;
                break;
            case Incave :
                curDir = Stop;
                break;
            case Outcave :
                curDir = this->strategy();
//                curDir = static_cast<dirstate> (rand() % 4);
  //              while(!canmove(preX,preY,curDir)){
    //                curDir = static_cast<dirstate> (rand() % 4);
      //          }
                break;
            case Backingcave :
                if(preX==init_posx && preY==init_posy){
                    state = Incave;
                    curDir = Stop;
                    outcave_time = 2000;
                } else{
                    curDir = Stop;
                    for(int d=0;d<4;++d)
                        if(nxt(preX,preY,d)!=Wall){
                            int nxtX=preX+deltax[d],nxtY=preY+deltay[d];
                            if(dis_map[nxtX][nxtY]==dis_map[preX][preY]-1){
                                curDir = static_cast<dirstate> (d);
                                break;
                            }
                        }
                }
                break;
            default: break;
        }
        //qDebug()<<state<<endl;
    }
    //if(state == Backingcave)
  //      qDebug() << "-------------------------------" << endl;
    //if(state == Outingcave) qDebug() << "curDir= " <<  curDir << " " << id << endl;
    //if(state == Outingcave) qDebug() << x() << " " << y() << "->" << x()+deltax[curDir] << y()+deltay[curDir] << endl;
    this -> setPos(x()+deltax[curDir],y()+deltay[curDir]);
//    qDebug() << x() << " " << y() << endl;
}//ghost?????????????????????????????????????????????????????????????????????????????????????????????
