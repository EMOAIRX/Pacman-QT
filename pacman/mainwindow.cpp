#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QString>
#include <Qpainter>
#include <QDebug>
#include <QLabel>
#include <QTimer>

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
    InitLabels();
    ui->graphicsView->setScene(game);
    //qDebug() << "233 " << endl;


}

void MainWindow::InitLabels()
{
    score_title = new QLabel(this);
    score_title->setText("score");
    score_title->setStyleSheet("QLabel {font-family: Fixedsys;color: black;font-size: 16px;}");
    score_title->setGeometry(50, 12, 60, 26);

    score = new QLabel(this);
    score->setIndent(-80);
    score->setText("0");
    score->setStyleSheet("QLabel {font-family: Fixedsys;color: black;font-size: 16px;}");
    score->setGeometry(110, 12, 150, 26);

    score_timer = new QTimer(this);
    score_timer->start(25);
    connect(score_timer,&QTimer::timeout, [=](){this->update_score();});

}

void MainWindow::update_score()
{
    score->setText(QString::number(game->Score));
    if (game->stat == Over) score_timer->stop();
    //if (game->stat == Game::Win) {
    //    win_label->show();
    //    score_timer->stop();
    //} else if (game->stat == Game::Lose) {
     //   lose_label->show();
     //   score_timer->stop();
    //}
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
