#ifndef PACMAN_H
#define PACMAN_H
#include "base.h"
#include <functional>
#include <QVector>

class Pacman : public Base
{
public:
    Pacman(int,int,Game*);
    ~Pacman();
    void obtain(int,int);
    void move();
    void Collision_determination();
    void caught();
    BaseH::pacmanstate get_state() const;
    friend class Game;
private:
    QTimer* pacnic_flash;
    int remain_panic_time,remain_panic_flash_time;
    Game* game;
    BaseH::pacmanstate state;
    QVector<QPixmap> animations[5];
    int animation_index;
};


#endif // PACMAN_H
