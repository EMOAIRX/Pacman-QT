#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include "pacman.h"
#include "ghost.h"
#include "base.h"

class Gamee : public QGraphicsScene
{
    Q_OBJECT

public:
    Gamee(QString);
    ~Gamee();
    Pacman* pacman;
    QTimer* pacman_timer;
    QThread* timerThread;
    void newpress(Qt::Key);
private slots:
    void pacman_handler();
};




#endif // GAME_H
