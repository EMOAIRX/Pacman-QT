#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QVector>
#include "pacman.h"
#include "ghost.h"
#include "base.h"


class Game : public QGraphicsScene
{
    Q_OBJECT

public:
    Game(int,int,QString);
    ~Game();
    Pacman* pacman;
    Ghost* ghost[4];
    QTimer* pacman_timer;
    QTimer* panic_timer,*ghost_timer[4];//Invincible time after taking drugs
    QTimer *powerball_flash_timer;//frequence of the ball flash
    QThread* timerThread;
    Base*** uimap;
    QVector<Base*> powerball;
    BaseH::stat stat;
    void newpress(Qt::Key);
    void obtain(int x,int y);
    void start();
    void over();
    void pause();

    int Score;
    int flash_tick;
    friend class Pacman;
    friend class Ghost;
private:
    void pacman_handler();
    void ghost_handler(int);

    void panic_handler();
    int panic_flash_tick;

    void powerball_flash();
};




#endif // GAME_H
