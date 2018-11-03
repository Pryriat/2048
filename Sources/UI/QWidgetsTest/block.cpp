#include "block.h"

QRectF block::boundingRect() const
{
    qreal penwidth = 1;
    return QRectF(-25-penwidth/2,-25-penwidth/2,
                  50+penwidth,50+penwidth);
}

void block::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // 为小方块使用贴图
    painter->drawPixmap(-25, -25, 75, 75, QPixmap(":/images/block.png"));
    painter->setBrush(brushcolor);
    QColor penColor = brushcolor;

    // 将颜色的透明度减小
    penColor.setAlpha(220);
    painter->setPen(penColor);
    painter->drawRect(-25, -25, 75, 75);
}


board::board()
{
    setFlags(QGraphicsItem::ItemIsFocusable);
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(moveOneStep()));
    curShape = en_DIY;
}

QRectF board::boundingRect() const
{
    qreal penwidth = 1;
    return QRectF(-25-penwidth/2,-25-penwidth/2,
                  75+penwidth,75+penwidth);
}

void board::clearBlock()
{
    qDebug() << "Func:clearBlock hit" <<endl;
}

void board::createBlock(const QPointF &point,blockShape shape)
{
    static const QColor colorTable[13] = {
        QColor(190, 190, 190, 100),     //0:Grey
        QColor(135, 205, 235, 100),     //2:SkyBlue
        QColor(100, 150, 235, 100),     //4:CornflowerBlue
        QColor(150, 250, 150, 100),     //8:PaleGreen
        QColor(50, 205, 50, 100),       //16:LimeGreen
        QColor(165, 42, 42, 100),       //32:Brown
        QColor(255, 99, 71, 100),       //64:Tomato
        QColor(255, 211, 155, 100),     //128:Burlywood1
        QColor(238, 197, 145, 100),     //256:Burlywood2
        QColor(205, 170, 125, 100),     //512:Burlywood3
        QColor(255, 165, 79, 100),      //1024:Tan1
        QColor(255, 215, 0, 100),       //2048:Gold
        QColor(255, 215, 0, 100)        //DIY:Gold
    };
    int shapeID = shape;
    QColor color;

    if(shape == en_DIY)
    {
        shapeID = qrand()%13;
    }
    color = colorTable[shapeID];
    curShape = shape;

    block *pBlock = new block(color);
    pBlock->setPos(-25,-25);

    if(isColliding())
    {
        stopTimer();
        emit gameFinished();
    }
    setPos(point);
}

bool board::isColliding()
{
    QList<QGraphicsItem *> itemList = childItems();
    QGraphicsItem *item;
    foreach (item, itemList) {
        if(item->collidingItems().count() > 1)
            return true;
    }
    return false;
}

blockShape board::getBlockShape()
{
    return curShape;
}

void board::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Down :
        moveBy(0, 60);
        if (isColliding()) {
            moveBy(0, -60);


            //clearBlock();


            emit needNewBlock();
        }
        break;

    case Qt::Key_Left :
        moveBy(-60, 0);
        if (isColliding())
            moveBy(60, 0);
        break;

    case Qt::Key_Right :
        moveBy(60, 0);
        if (isColliding())
            moveBy(-60, 0);
        break;

    case Qt::Key_Space :
        moveBy(0, 60);
        while (!isColliding()) {
            moveBy(0, 60);
        }
        moveBy(0, -60);
        //clearBlock();
        emit needNewBlock();
        break;
    }
}

void board::startTimer(int interval)
{
    timer->start(interval);
}

void board::moveOneStep()
{
    moveBy(0, 60);
    if (isColliding())
    {
        moveBy(0, -60);
        //clearBlock();
        emit needNewBlock();
    }
}

void board::stopTimer()
{
    timer->stop();
}
