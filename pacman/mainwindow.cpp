#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QString>
#include <Qpainter>
#include <QDebug>

using namespace BaseH;
#define W BaseH::ObjectWidth


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("pac-man"));
    ui->graphicsView->setStyleSheet("QGraphicsView {border: none;}");
    ui->graphicsView->setBackgroundBrush(Qt::black);
    ui->graphicsView->setFocusPolicy(Qt::NoFocus);
    int w = 27, h = 25;
    ui->graphicsView->setGeometry(50,50,w*20,h*20);
    //qDebug() << "233 " << endl;
    game = new Game(w,h,":/map/map1.txt");
    ui->graphicsView->setScene(game);
    //qDebug() << "233 " << endl;


}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    //qDebug() << gamee -> pacman_timer -> remainingTime();
    game -> newpress( static_cast<Qt::Key> (e -> key()) );

}


MainWindow::~MainWindow()
{
    delete ui;
}
