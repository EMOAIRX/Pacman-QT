#include "game.h"
#include <vector>
#include <functional>
#include<QDebug>
//参考 https://www.mentalfloss.com/article/90874/your-pac-man-game-learning-different-strategies-each-ghost
//一个策略是这样的
using std::vector;
typedef std::pair<int,int> pii;
#define fi first
#define se second
#define dr pacman -> get_curDir()
using namespace BaseH;
#define nxt Base::nxt
dirstate deter_dir(std::function<pii()> gdist,int x,int y,dirstate curdir){
    static vector<dirstate> lis;
    pii P = gdist();
    static int ppp=0;
    ppp++;
#define xx P.first
#define yy P.second
    /*enum dirstate{Left=0,Right=1,Up=2,Down=3,Stop=4};
    x负
    y正
    往左*/
    qDebug() << P.fi << " " << P.se << "  --  " << ppp;
    if(abs(P.fi)>abs(P.se))
        lis = vector<dirstate>{xx>0?Right:Left,yy>0?Down:Up,yy>0?Up:Down,xx>0?Left:Right};
    else
        lis = vector<dirstate>{yy>0?Down:Up,xx>0?Right:Left,xx>0?Left:Right,yy>0?Up:Down};
    for (auto d : lis){
        if(d==(curdir^1)) continue;
        if (nxt(x,y,d) == Wall || nxt(x,y,d) == Door) continue;
        return d;
    }
    return Stop;
};
dirstate random(int x,int y){
    auto d = static_cast<dirstate> (rand() % 4);
    auto v  = nxt(x,y,d);
    while(v == Wall || v == Door){
        d = static_cast<dirstate>(rand() % 4);
        v  = nxt(x,y,d);
    }
    return d;
};
std::function<BaseH::dirstate()> chasing_red(Ghost *ghost,Pacman *pacman){
std::function<pii()> gdist = [=]() -> pii const{
    return {pacman->get_x() - ghost->get_x()
           ,pacman->get_y() - ghost->get_y()};
};
return [=](){return deter_dir(gdist,ghost->get_x(),ghost->get_y(),ghost->get_curDir());};
};

std::function<BaseH::dirstate()> chasing_pink(Ghost *ghost,Pacman *pacman){
std::function<pii()> gdist = [=]() -> pii const{
    return {pacman->get_x()+(deltax[dr]*4) - ghost->get_x()
           ,pacman->get_y()+(deltay[dr]*4) - ghost->get_y()};
};
return [=](){return deter_dir(gdist,ghost->get_x(),ghost->get_y(),ghost->get_curDir());};
};

std::function<BaseH::dirstate()> chasing_orange(Ghost *ghost,Ghost* ghost_red,Pacman *pacman){
std::function<pii()> gdist = [=]() -> pii const{
    return {(pacman->get_x()*2 - ghost_red->get_x()) - ghost->get_x()
           ,(pacman->get_y()*2 - ghost_red->get_y()) - ghost->get_y()};
};
return [=](){return deter_dir(gdist,ghost->get_x(),ghost->get_y(),ghost->get_curDir());};
};

std::function<BaseH::dirstate()> chasing_blue(Ghost *ghost,Pacman *pacman){
std::function<pii()> gdist = [=]() -> pii const{
    return {pacman->get_x() - ghost->get_x()
            ,pacman->get_y() - ghost->get_y()};
};
return [=](){
    int disx,disy;
    disx = pacman->get_x() - ghost->get_x();
    disy = pacman->get_y() - ghost->get_y();
    //if(disx*disx+disy*disy<32) return random(ghost->get_x(),ghost->get_y());
    //    else
    return deter_dir(gdist,ghost->get_x(),ghost->get_y(),ghost->get_curDir());
};
};


