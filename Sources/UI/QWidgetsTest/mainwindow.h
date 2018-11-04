#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "public.h"
#include "block.h"
#include <QMainWindow>
#include <QGraphicsView>

class MainWindow : public QGraphicsView
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    void keyPressEvent(QKeyEvent *event);

public slots:
    void startGame();
    void clearFullRows();
    void moveBox();
    void gameOver();
    void restartGame();
    void finishGame();
    void pauseGame();
    void returnGame();

private:
    board *pBoard;
    board *nextBoard;
    QGraphicsLineItem *topLine;
    QGraphicsLineItem *bottomLine;
    QGraphicsLineItem *leftLine;
    QGraphicsLineItem *rightLine;
    qreal gameSpeed;
    QList<int> rows;
    void initView();
    void initGame();
    void updateScore(const int fullRowNum = 0);

    QGraphicsTextItem *gameScoreText;
    QGraphicsTextItem *gameLevelText;

    // 遮罩面板
    QGraphicsWidget *maskWidget;

    // 各种按钮
    QGraphicsWidget *startButton;
    QGraphicsWidget *finishButton;
    QGraphicsWidget *restartButton;
    QGraphicsWidget *pauseButton;
    QGraphicsWidget *optionButton;
    QGraphicsWidget *returnButton;
    QGraphicsWidget *helpButton;
    QGraphicsWidget *exitButton;
    QGraphicsWidget *showMenuButton;
    QGraphicsWidget *scoreBoardButton;

    // 各种文本
    QGraphicsTextItem *gameWelcomeText;
    QGraphicsTextItem *gamePausedText;
    QGraphicsTextItem *gameOverText;

    QMediaPlayer *backgroundMusic;
    QMediaPlayer *clearRowSound;

};

#endif // MAINWINDOW_H
