#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QString>
#include <Qpainter>
#include <QDebug>

using namespace BaseH;
#define W BaseH::ObjectWidth


/*void MainWindow::paintEvent(QPaintEvent *event){
    Q_UNUSED(event);
    QPainter painter(this);
    painter.begin(this);
    qDebug() << startX << " " << startY << " " << startX+W*Width << " " << startY+W*Height << endl;
    painter.drawRect(startX,startY,startX+W*Width,startY+W*Height);
    painter.end();
}*/

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(tr("pac-man"));
    ui->graphicsView->setStyleSheet("QGraphicsView {border: none;}");
    ui->graphicsView->setBackgroundBrush(Qt::black);
    ui->graphicsView->setFocusPolicy(Qt::NoFocus);
    ui->graphicsView->setGeometry(1,1,1000,1000);
    //qDebug() << "233 " << endl;
    gamee = new Gamee(":/map/map1.txt");
    ui->graphicsView->setScene(gamee);
    //qDebug() << "233 " << endl;


}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    //qDebug() << gamee -> pacman_timer -> remainingTime();
    gamee -> newpress( static_cast<Qt::Key> (e -> key()) );

}


MainWindow::~MainWindow()
{
    delete ui;
}
