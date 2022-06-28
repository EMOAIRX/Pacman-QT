#ifndef GHOST_H
#define GHOST_H
#include "base.h"
#include <functional>
#include <QVector>

std::function<BaseH::dirstate()> chasing_red(Ghost*,Pacman*);
std::function<BaseH::dirstate()> chasing_pink(Ghost*,Pacman*);
std::function<BaseH::dirstate()> chasing_orange(Ghost*,Ghost*,Pacman*);
std::function<BaseH::dirstate()> chasing_blue(Ghost*,Pacman*);

class Ghost : public Base
{
public:
//    Ghost();
    Ghost(int,int,int,int,Game*);
//    ~Ghost();
    void obtain(int,int);
    void move();
    void caught();
    BaseH::ghoststate get_state();
    BaseH::dirstate faceto;
    friend class Game;
private:
    bool canmove(int, int, BaseH::dirstate);
    std::function<BaseH::dirstate()> strategy;
    Game* game;
    int id;
    QVector<QPixmap> animations[5];
    int animation_index;
    BaseH::ghoststate state;
    int outcave_time;
    int dis_map[35][35];
    void get_dis_map();
};


#endif // GHOST_H


