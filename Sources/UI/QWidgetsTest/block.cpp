#include "board.h"
#include "number_block.h"
#include "framework_base.h"

QRectF block::boundingRect() const
{
    qreal penwidth = 1;
    return QRectF(0-penwidth/2,
                  0-penwidth/2,
                  DE_BLOCKLENGTH+penwidth,
                  DE_BLOCKLENGTH+penwidth);
}

void block::paint(QPainter *painter,
                  const QStyleOptionGraphicsItem *option,
                  QWidget *widget)
{
    switch(shape)
    {
    case en_2:
    {
        painter->drawPixmap(0, 0, DE_BLOCKLENGTH, DE_BLOCKLENGTH, QPixmap(":/images/block_2.png"));
    }break;

    case en_4:
    {
        painter->drawPixmap(0, 0, DE_BLOCKLENGTH, DE_BLOCKLENGTH, QPixmap(":/images/block_4.png"));
    }break;

    case en_8:
    {
        painter->drawPixmap(0, 0, DE_BLOCKLENGTH, DE_BLOCKLENGTH, QPixmap(":/images/block_8.png"));
    }break;

    case en_16:
    {
        painter->drawPixmap(0, 0, DE_BLOCKLENGTH, DE_BLOCKLENGTH, QPixmap(":/images/block_16.png"));
    }break;

    case en_32:
    {
        painter->drawPixmap(0, 0, DE_BLOCKLENGTH, DE_BLOCKLENGTH, QPixmap(":/images/block_32.png"));
    }break;

    case en_64:
    {
        painter->drawPixmap(0, 0, DE_BLOCKLENGTH, DE_BLOCKLENGTH, QPixmap(":/images/block_64.png"));
    }break;

    case en_128:
    {
        painter->drawPixmap(0, 0, DE_BLOCKLENGTH, DE_BLOCKLENGTH, QPixmap(":/images/block_128.png"));
    }break;

    case en_256:
    {
        painter->drawPixmap(0, 0, DE_BLOCKLENGTH, DE_BLOCKLENGTH, QPixmap(":/images/block_256.png"));
    }break;

    case en_512:
    {
        painter->drawPixmap(0, 0, DE_BLOCKLENGTH, DE_BLOCKLENGTH, QPixmap(":/images/block_512.png"));
    }break;

    case en_1024:
    {
        painter->drawPixmap(0, 0, DE_BLOCKLENGTH, DE_BLOCKLENGTH, QPixmap(":/images/block_1024.png"));
    }break;

    case en_2048:
    {
        painter->drawPixmap(0, 0, DE_BLOCKLENGTH, DE_BLOCKLENGTH, QPixmap(":/images/block_2048.png"));
    }break;

    case en_4096:
    {
        painter->drawPixmap(0, 0, DE_BLOCKLENGTH, DE_BLOCKLENGTH, QPixmap(":/images/block_4096.png"));
    }break;

    default:
    {
        painter->drawPixmap(0, 0, DE_BLOCKLENGTH, DE_BLOCKLENGTH, QPixmap(":/images/block_Empty.png"));
    }
    }
}

blockShape block::getShape()   //获取当前方块的类型
{
    return this->shape;
}

void block::setShape(blockShape _shape)    //设置当前方块类型
{
    shape = _shape;
}
