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
    QPushButton *playagain;

    void InitLabels();
    void update_score();

protected:
    void keyPressEvent(QKeyEvent *);
    //virtual void paintEvent(QPaintEvent *event);
private slots:
    void button_pushed();
};

#endif // MAINWINDOW_H
