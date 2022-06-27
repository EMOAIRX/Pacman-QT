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

    game = new Game(w,h,":/map/map1.txt",this);
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

    win_label = new QLabel(this);
    win_label->setText("Victory!");
    win_label->setStyleSheet("QLabel {font-family: Fixedsys;color: black;font-size: 16px;}");
    win_label->setGeometry(50,24,70,26);
    win_label->hide();

    lose_label = new QLabel(this);
    lose_label->setText("Defeat");
    lose_label->setStyleSheet("QLabel {font-family: Fixedsys;color: black;font-size: 16px;}");
    lose_label->setGeometry(50,24,70,26);
    lose_label->hide();

    playagain = new QLabel(this);
    playagain->setGeometry(300,20,500,40);
    playagain->setText("Press Enter to Play Again");
    playagain->setStyleSheet("QLabel {font-family: Fixedsys;color: red;font-size: 32px;}");
    playagain->hide();
//    playagain = new QPushButton("playagain",this);
//    connect(playagain,SIGNAL(clicked()),this,SLOT(button_pushed()));
    //connect(playagain,SIGNAL(clicked()),playagain,);
}

void MainWindow::button_pushed()
{
    int w = 27, h = 25;
    lose_label->hide();
    win_label->hide();
    score_timer->start(25);
    game->replay(w,h,":/map/map1.txt");

}

void MainWindow::update_score()
{
    score->setText(QString::number(game->Score));
    if (game->stat == Over)
    {
        score_timer->stop();
        if (game->died < 3)
            win_label->show();
        else
           lose_label->show();
        playagain->show();
    }
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
