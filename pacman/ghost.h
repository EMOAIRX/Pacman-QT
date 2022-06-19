#ifndef GHOST_H
#define GHOST_H
#include "base.h"


class Ghost : public Base
{
public:
//    Ghost();
    Ghost(int,int,int,Game*);
//    ~Ghost();
    void obtain(int,int);
    void move();
    friend class Game;
private:
    bool canmove(int, int, BaseH::dirstate);
    Game* game;
    int id;
    BaseH::ghoststate state;
    int outcave_time;
};


#endif // GHOST_H


