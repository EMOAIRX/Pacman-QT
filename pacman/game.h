#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
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
    QThread* timerThread;
    Base*** uimap;
    void newpress(Qt::Key);
    void obtain(int x,int y);
    void start();

    friend class Pacman;
    friend class Ghost;
private:
    void pacman_handler();
    void ghost_handler(int);
};




#endif // GAME_H
