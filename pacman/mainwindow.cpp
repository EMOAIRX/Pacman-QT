#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QString>
#include <Qpainter>
#include <QDebug>
#include <QLabel>
#include <QTimer>
#define redheart QPixmap(":/images/love_heart.png")
#define blankheart QPixmap(":/images/blank_heart.png")

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
    score->setGeometry(100, 12, 150, 26);

    score_timer = new QTimer(this);
    score_timer->start(25);
    connect(score_timer,&QTimer::timeout, [=](){this->update_score();});

    win_label = new QLabel(this);
    win_label->setText("Victory!");
    win_label->setStyleSheet("QLabel {font-family: Fixedsys;color: blue;font-size: 32px;}");
    win_label->setGeometry(150,20,150,40);
    win_label->hide();

    lose_label = new QLabel(this);
    lose_label->setText("Defeat.");
    lose_label->setStyleSheet("QLabel {font-family: Fixedsys;color: black;font-size: 32px;}");
    lose_label->setGeometry(150,20,150,40);
    lose_label->hide();

    playagain = new QLabel(this);
    playagain->setGeometry(280,20,500,40);
    playagain->setText("Press Enter to Play Again");
    playagain->setStyleSheet("QLabel {font-family: Fixedsys;color: red;font-size: 32px;}");
    playagain->hide();


    remain1 = new QLabel(this);
    remain2 = new QLabel(this);
    remain3 = new QLabel(this);
    remain1->setFixedSize(30,29);
    remain2->setFixedSize(30,29);
    remain3->setFixedSize(30,29);
    //remain1->setScaledContents(true);
    //remain2->setScaledContents(true);
    //remain3->setScaledContents(true);
    remain1->setPixmap(redheart);
    remain2->setPixmap(redheart);
    remain3->setPixmap(redheart);
    remain1->setGeometry(50,35,30,29);
    remain2->setGeometry(80,35,30,29);
    remain3->setGeometry(110,35,30,29);

}


void MainWindow::update_score()
{
    score->setText(QString::number(game->Score));
    if (game->died > 0) remain3->setPixmap(blankheart);
    else remain3->setPixmap(redheart);
    if (game->died > 1) remain2->setPixmap(blankheart);
    else remain2->setPixmap(redheart);
    if (game->died > 2) remain1->setPixmap(blankheart);
    else remain1->setPixmap(redheart);
    if (game->stat == Over)
    {
        score_timer->stop();
        if (game->died < 3)
            win_label->show();
        else
           lose_label->show();
        qDebug()<<game->died<<endl;
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
