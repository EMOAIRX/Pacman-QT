#ifndef PACMAN_H
#define PACMAN_H
#include "base.h"
class Pacman : public Base
{
public:
    Pacman(int,int);
    ~Pacman();
    void move();
};


#endif // PACMAN_H
