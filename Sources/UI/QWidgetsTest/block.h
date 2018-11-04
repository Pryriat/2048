#ifndef BLOCK_H
#define BLOCK_H

#include"public.h"



class block : public QGraphicsObject
{
Q_OBJECT
public:
    block(const QColor &_brushcolor):brushcolor(_brushcolor)
    {

    }
    QRectF boundingRect() const;    //形状构造
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget); //具体绘图

private:
    QColor brushcolor;  //刷子颜色
};

class board : public QObject,public QGraphicsItemGroup
{
    Q_OBJECT
public:

    board();
    QRectF boundingRect() const;

    bool isColliding();
    void createBlock(const QPointF &point = QPointF(0, 0), blockShape shape = en_DIY);
    void clearBlock();

    blockShape getBlockShape();

protected:
    void keyPressEvent(QKeyEvent *event);

signals:
    void needNewBlock();
    void gameFinished();

public slots:
    void moveOneStep();
    void startTimer(int interval);
    void stopTimer();

private:
    blockShape curShape;
    QTransform curTransform;
    QTimer *timer;

};





#endif // BLOCK_H
