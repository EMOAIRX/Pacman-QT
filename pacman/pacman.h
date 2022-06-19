#ifndef PACMAN_H
#define PACMAN_H
#include "base.h"

class Pacman : public Base
{
public:
    Pacman(int,int,Game*);
    ~Pacman();
    void obtain(int,int);
    void move();
    friend class Game;
private:
    Game* game;
    BaseH::pacmanstate state;
};


#endif // PACMAN_H
