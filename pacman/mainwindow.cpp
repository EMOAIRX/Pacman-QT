#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QString>
#include <Qpainter>
#include <QDebug>

using namespace BaseH;
#define W BaseH::ObjectWidth


void MainWindow::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    painter.begin(this);
    qDebug() << startX << " " << startY << " " << startX+W*Width << " " << startY+W*Height << endl;
    painter.drawRect(startX,startY,startX+W*Width,startY+W*Height);
    painter.end();
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug() << "233 " << endl;
    game = new Game();
    qDebug() << "233 " << endl;


}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    qDebug() << game -> pacman_timer -> remainingTime();
    game -> newpress( static_cast<Qt::Key> (e -> key()) );

}


MainWindow::~MainWindow()
{
    delete ui;
}
