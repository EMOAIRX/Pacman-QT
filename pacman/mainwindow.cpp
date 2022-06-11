#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
//   int x = ui->pushButton->x();
//   int y = ui->pushButton->y();
    QString p;
    p.setNum(e -> key(),10);
    ui->Key_event->setText(p);
//   switch (e->key())
//   {
//   case Qt::Key_W : ui->pushButton->move(x, y-10); break;
//   case Qt::Key_S : ui->pushButton->move(x, y+10); break;
//   case Qt::Key_A : ui->pushButton->move(x-10, y); break;
//   case Qt::Key_D : ui->pushButton->move(x+10, y); break;
//   }
}


void MainWindow::mousePressEvent(QMouseEvent *e)
{
//   int x = ui->pushButton->x();
//   int y = ui->pushButton->y();
    QString p;
    p.setNum((e->x())*10000+(e->y()),10);
    ui->Key_event->setText(p);
//   switch (e->key())
//   {
//   case Qt::Key_W : ui->pushButton->move(x, y-10); break;
//   case Qt::Key_S : ui->pushButton->move(x, y+10); break;
//   case Qt::Key_A : ui->pushButton->move(x-10, y); break;
//   case Qt::Key_D : ui->pushButton->move(x+10, y); break;
//   }
}


MainWindow::~MainWindow()
{
    delete ui;
}
