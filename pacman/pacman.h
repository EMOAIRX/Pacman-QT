#ifndef PACMAN_H
#define PACMAN_H
#include "base.h"
#include <functional>

class Pacman : public Base
{
public:
    Pacman(int,int,Game*);
    ~Pacman();
    void obtain(int,int);
    void move();
    void Collision_determination();
    void caught();
    friend class Game;
private:

    QTimer* pacnic_flash;
    int remain_panic_time,remain_panic_flash_time;
    Game* game;
    BaseH::pacmanstate state;
};


#endif // PACMAN_H
