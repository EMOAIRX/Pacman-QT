#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
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
    QLabel *score,*score_title;
    QTimer *score_timer;
    void InitLabels();
    void update_score();

protected:
    void keyPressEvent(QKeyEvent *);
    //virtual void paintEvent(QPaintEvent *event);

};

#endif // MAINWINDOW_H
