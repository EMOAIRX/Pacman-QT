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
    QTimer* pacman_timer;
    QThread* timerThread;
    Base*** uimap;
    void newpress(Qt::Key);
private slots:
    void pacman_handler();
};




#endif // GAME_H
