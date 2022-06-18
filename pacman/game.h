#ifndef GAME_H
#define GAME_H

#include "pacman.h"
#include "ghost.h"
#include "base.h"

class Game : public QObject
{
public:
    Game();
    Pacman* pacman;
    QTimer* pacman_timer;
    QThread* timerThread;
    void newpress(Qt::Key);
private slots:
    void pacman_handler();
};




#endif // GAME_H
