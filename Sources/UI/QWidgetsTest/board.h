#ifndef BOARD_H
#define BOARD_H

#include "number_block.h"
#include "framework_base.h"
#include "public.h"

class block : public QGraphicsObject
{
Q_OBJECT
public:
    block(const blockShape _shape):shape(_shape){ }
    QRectF boundingRect() const;    //形状构造
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget); //具体绘图
    blockShape getShape();   //获取当前方块的类型
    void setShape(blockShape _shape);   //设置当前方块类型

private:
    blockShape shape;   //存储当前方块的类型
};



class board : public QObject, public QGraphicsItemGroup
{
    Q_OBJECT
public:

    board();
    QRectF boundingRect() const;        //棋盘图形构造
    void displayBoard();                //将框架中方块阵列以图形的形式映射到屏幕
    void setCurBlock(block *pBlock);    //设置当前控制方块图形类指针curBlock
    void clearBlock();

    void paintBoard(const QPointF &BoardPos);                  //画面刷新


    blockShape numToShape(unsigned int num);     //数字转换为方块类型
    int shapeToNum(blockShape shape);   //方块类型转换为数字

    bool isColliding(); //碰撞判断
    void createBlock(blockShape _shape = en_empty);  //方块生成构造

    blockShape getBlockShape(); //获取当前方块的类型

    framework *getFrameworkPtr()   //返回pFramework指针值
    {
        return pFramework;
    }
    QTimer *getTimer()  //返回timer指针值
    {
        return timer;
    }
    void keyPressEvent(QKeyEvent *event);   //按键事件函数

signals:
    void boardChange();
    void needNewBlock();
    void gameFinished();

public slots:
    void moveOneStep();
    void startTimer(int interval);
    void stopTimer();

private:
    framework *pFramework;
    block *curBlock;
    QTransform curTransform;
    QTimer *timer;

};


#endif // BOARD_H
