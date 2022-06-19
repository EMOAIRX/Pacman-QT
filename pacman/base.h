#ifndef BASE_H
#define BASE_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QTimer>
#include <QVector>
class Game;
namespace BaseH{
    enum mapstate{Space=0,Wall=1,Food=2,Medicine=3,Door=4};
    enum pacmanstate{Normal=0,Panic=1};
    enum ghoststate{Incave=0,Outcave=1,Backcave=2};
    enum dirstate{Left=0,Right=1,Up=2,Down=3,Stop=4};
    extern const int deltax[5];
    extern const int deltay[5];
    extern int Width,Height,ObjectWidth;
    extern int startX,startY;
    extern int map[35][35];
    extern const int INTERVAL_ghost,INTERVAL_pacman;
}

class Base:public QGraphicsPixmapItem
{
public:
    Base(QPixmap);
    Base();
    void set_nxtDir(BaseH::dirstate);
    void set_curDir(BaseH::dirstate);
    static BaseH::mapstate nxt(int,int,BaseH::dirstate);
    bool canmove(int,int,BaseH::dirstate);
    //virtual void move();

protected:
    BaseH::dirstate curDir,nxtDir;
    int preX,preY;//表示之前在地图中具体的位置
};

#endif // BASE_H
