#ifndef PACMAN_H
#define PACMAN_H
#include "base.h"
class Game;

class Pacman : public Base
{
public:
    Pacman(int,int,Game*);
    ~Pacman();
    void move();
    void obtain(int,int);
    friend class Game;
private:
    Game* game;
    BaseH::pacmanstate state;
};


#endif // PACMAN_H
