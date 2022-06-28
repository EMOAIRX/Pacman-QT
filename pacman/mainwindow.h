#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QPushButton>
#include <game.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    Game *game;
    QLabel *score,*score_title,*win_label,*lose_label;
    QTimer *score_timer;
    QLabel *playagain;
    QLabel *remain1,*remain2,*remain3;
    void InitLabels();
    void update_score();
    friend class Game;

protected:
    void keyPressEvent(QKeyEvent *);
    //virtual void paintEvent(QPaintEvent *event);
//private slots:
//    void button_pushed();
};

#endif // MAINWINDOW_H
