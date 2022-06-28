#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QVector>
#include "pacman.h"
#include "ghost.h"
#include "base.h"

class MainWindow;
class Game : public QGraphicsScene
{
    Q_OBJECT

public:
    Game(int,int,QString,MainWindow*);
    ~Game();
    MainWindow* mainwindow;
    Pacman* pacman;
    Ghost* ghost[4];
    QTimer* pacman_timer, *pacman_animation_timer;
    QTimer* panic_timer,*ghost_timer[4], *ghost_animation_timer[4];//Invincible time after taking drugs
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
    void win();
    void lose();
    void replay(int,int,QString);
    int died;
    int Score;
    int flash_tick;
    friend class Pacman;
    friend class Ghost;
private:
    int Lposx,Lposy,Rposx,Rposy;
    void pacman_handler();
    void pacman_animation_handler();
    void ghost_handler(int);
    void ghost_animation_handler(int);

    void panic_handler();
    int panic_flash_tick;
    int remain_food,remain_medicine;
    void powerball_flash();
};




#endif // GAME_H
