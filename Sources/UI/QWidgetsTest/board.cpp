#include "public.h"
#include "board.h"

board::board()
{
    setFlags(QGraphicsItem::ItemIsFocusable);
    timer = new QTimer(this);
    //connect(timer, SIGNAL(timeout()), this, SLOT(moveOneStep()));   //将计时器信号与自由下落函数连接，用于驱动方块自由下落
    curBlock = nullptr;
    pFramework = new framework(DE_DIFFICULITY);  //框架类构造，难度暂调整为1(GL_DIFFICULITY)
}

void board::displayBoard()
{

    framework_block_item *pItem = pFramework->current_status();
    blockShape shape = en_empty;
    block *pBlock = nullptr;
    for(int r = 0; r < 7; ++r)
    {
       for(int c = 0; c < 5; ++c)
       {
           if(pItem[c].is_none == false)
           {
               if(pItem[c].is_uncombined == false)  //数值块
               {
                   shape = numToShape(pItem[c].block->get_number());
               }
               else //障碍块
               {
                   shape = en_empty;
               }

               pBlock = new block(shape);
               addToGroup(pBlock);
               pBlock->setPos(c * DE_BLOCKLENGTH,
                              (7 - r) * DE_BLOCKLENGTH);
           }

       }
       ++pItem; //调至下一行
    }

}

void board::setCurBlock(block *pBlock)      //设置当前控制方块图形类指针curBlock
{
    if(pBlock == nullptr)
    {
        return;
    }
    curBlock = pBlock;
}

QRectF board::boundingRect() const  //图形界面棋盘大小
{
    qreal penwidth = 1;
    return QRectF(0 - penwidth/2,
                  0 - penwidth/2,
                  DE_BLOCKLENGTH * 5 + penwidth,
                  DE_BLOCKLENGTH * 7 + penwidth);
}

blockShape board::numToShape(unsigned int num)   //数字转换为方块类型
{
    blockShape shape = en_empty;
    switch (num)
    {

    case 0:
    {
        shape = en_empty;
    }break;

    case 2:
    {
        shape = en_2;
    }break;

    case 4:
    {
        shape = en_4;
    }break;

    case 8:
    {
        shape = en_8;
    }break;

    case 16:
    {
        shape = en_16;
    }break;

    case 32:
    {
        shape = en_32;
    }break;

    case 64:
    {
        shape = en_64;
    }break;

    case 128:
    {
        shape = en_128;
    }break;

    case 256:
    {
        shape = en_256;
    }break;

    case 512:
    {
        shape = en_512;
    }break;

    case 1024:
    {
        shape = en_1024;
    }break;

    case 2048:
    {
        shape = en_2048;
    }break;

    case 4096:
    {
        shape = en_4096;
    }break;

    default:
    {
        shape = en_empty;
    }

    }
    return shape;
}

int board::shapeToNum(blockShape shape) //方块类型转换为数字
{
    int num = 0;
    switch(shape)
    {
    case en_2:
    {
        num = 2;
    }break;

    case en_4:
    {
        num = 4;
    }break;

    case en_8:
    {
        num = 8;
    }break;

    case en_16:
    {
        num = 16;
    }break;

    case en_32:
    {
        num = 32;
    }break;

    case en_64:
    {
        num = 64;
    }break;

    case en_128:
    {
        num = 128;
    }break;

    case en_256:
    {
        num = 256;
    }break;

    case en_512:
    {
        num = 512;
    }break;

    case en_1024:
    {
        num = 1024;
    }break;

    case en_2048:
    {
        num = 2048;
    }break;

    case en_4096:
    {
        num = 4096;
    }break;

    default:
    {
        num = 0;
    }
    }
    return 0;
}

void board::clearBlock()
{
    //原界面清空
    QList<QGraphicsItem *> itemList = childItems();
    QGraphicsItem *item;
    block *OneBlock = nullptr;
    foreach (item, itemList)
    {
        removeFromGroup(item);
        OneBlock = dynamic_cast<block *>(item);
        OneBlock->deleteLater();
    }
    qDebug() << "Func:clearBlock hit" <<endl;
}

void board::createBlock(blockShape _shape)  //根据指定的位置point和类型shape生成方块，并显示于界面中
{
    //框架的方块构造
    number_block * pNumberBlock = pFramework->generate_block();
    blockShape shape = en_empty;

    if(_shape == en_empty)  //未指定类型时即随机构造
    {
        shape = numToShape(pNumberBlock->get_number());
    }
    else    //若指定类型则根据方块类型进行构造
    {
        shape = _shape;
        pNumberBlock->modify_number(shapeToNum(shape));
    }
    pFramework->setMovingBlock(pNumberBlock);


    //图形界面的方块构造
    block *pBlock = new block(shape);
    setCurBlock(pBlock);
    pBlock->setPos(pNumberBlock->get_x()*DE_BLOCKLENGTH,
                   (7 - pNumberBlock->get_y())*DE_BLOCKLENGTH);
    //addToGroup(pBlock); //可能会有bug

}

bool board::isColliding()
{
    /*
    QList<QGraphicsItem *> itemList = childItems();
    QGraphicsItem *item;
    foreach (item, itemList) {
        if(item->collidingItems().count() > 1)
            return true;
    }
    return false;*/
    return false;
}

blockShape board::getBlockShape()
{
    return curBlock->getShape();
}

void board::keyPressEvent(QKeyEvent *event)
{
    //qDebug()<<"Func:board::keyPressEvent hits!"<<endl;

    unsigned int tmp;
    unsigned int current_x = pFramework->moving_block->get_x();
    unsigned int current_y = pFramework->moving_block->get_y();
    framework_block_item *pBlock = pFramework->current_status();
    switch (event->key())
    {
    case Qt::Key_Down :
    {
        /*
        pFramework->control(80);
        curBlock->moveBy(0, 60);
        qDebug()<<"Key_Down hits"<<endl;
        if (isColliding()) {
            curBlock->moveBy(0, -60);

            //clearBlock();

            emit needNewBlock();
        }*/
        tmp = 0;
        if(0 == current_y)
        {//触底判断
            return;
        }

        if(!pBlock[DE_BOARD_ROW*current_x + current_y - 1].is_none ||
                pBlock[DE_BOARD_ROW*current_x + current_y - 1].is_uncombined)
        {//如果方块下为非空块或为障碍块
            return;
        }

        while(!pBlock[DE_BOARD_ROW*current_x + tmp].is_none)
        {//从最方块所在列的最下一行开始遍历，找到为空的第一行
            tmp++;
        }

        pBlock[DE_BOARD_ROW*current_x + current_y].is_none = true;
        pBlock[DE_BOARD_ROW*current_x + current_y].block = nullptr;
        pFramework->moving_block->modify_y(tmp);
        pBlock[DE_BOARD_ROW*current_x + tmp].block = pFramework->moving_block;
        pBlock[DE_BOARD_ROW*current_x + tmp].is_none = false;
        pBlock[DE_BOARD_ROW*current_x + tmp].is_uncombined = false;

        qDebug()<<"Key_Down hits!"<<endl;

    }break;

    case Qt::Key_Left :
    {
        /*
        pFramework->control(75);
        curBlock->moveBy(-60, 0);
        if (isColliding())
            curBlock->moveBy(60, 0);
        */
        if(0 == current_x)
        {//边界判断
            return;
        }

        if(!pBlock[DE_BOARD_ROW*(current_x - 1) + current_y].is_none ||
                pBlock[DE_BOARD_ROW*(current_x - 1) + current_y].is_uncombined)
        {//左侧块非空或为障碍块
            return;
        }
        pBlock[(current_x * DE_BOARD_ROW) + current_y].is_none = true;
        pBlock[(current_x * DE_BOARD_ROW) + current_y].block = nullptr;
        pBlock[DE_BOARD_ROW*(current_x - 1) + current_y].is_none = false;
        pBlock[DE_BOARD_ROW*(current_x - 1) + current_y].block = pFramework->moving_block;
        pFramework->moving_block->modify_x(current_x - 1);

        qDebug()<<"Key_Left hits!"<<endl;
    }break;

    case Qt::Key_Right :
    {
        /*
        pFramework->control(77);
        curBlock->moveBy(60, 0);
        if (isColliding())
            curBlock->moveBy(-60, 0);
        */
        if(current_x == 5 - 1)
        {//右边界判断
            return;
        }
        if(!pBlock[DE_BOARD_ROW*(current_x + 1) + current_y].is_none ||
                pBlock[DE_BOARD_ROW*(current_x + 1) + current_y].is_uncombined)
        {//右侧块不为空或为障碍块
            return;
        }
        pBlock[(current_x * DE_BOARD_ROW) + current_y].is_none = true;
        pBlock[(current_x * DE_BOARD_ROW) + current_y].block = nullptr;
        pBlock[DE_BOARD_ROW*(current_x + 1) + current_y].is_none = false;
        pBlock[DE_BOARD_ROW*(current_x + 1) + current_y].block = pFramework->moving_block;
        pFramework->moving_block->modify_x(current_x + 1);
        qDebug()<<"Key_Right hits!"<<endl;
    }break;

    }
    paintBoard(QPoint(DE_BOARD_XPOS,DE_BOARD_YPOS));
    emit boardChange();
}

void board::startTimer(int interval)
{
    timer->start(interval);
}

void board::moveOneStep()
{
    qDebug()<<"Func:moveOneStep hits"<<endl;
    curBlock->moveBy(0, 60);
    if (isColliding())
    {
        curBlock->moveBy(0, -60);
        //clearBlock();
        emit needNewBlock();
    }
}

void board::stopTimer()
{
    timer->stop();
}

void board::paintBoard(const QPointF &BoardPos)  //画面刷新
{
    hide();
    //原界面清空
    QList<QGraphicsItem *> itemList = childItems();
    QGraphicsItem *item;
    block *OneBlock = nullptr;
    foreach (item, itemList)
    {
        removeFromGroup(item);
        OneBlock = (block *)item;
        OneBlock->deleteLater();
    }

    //新界面显示
    setPos(BoardPos);   //设置棋盘坐标
    framework_block_item *pBlocks = pFramework->current_status();
    OneBlock = nullptr;
    int times;
    blockShape bShape;
    for(times = 0; times < DE_BOARD_COLUMN*DE_BOARD_ROW; ++times)
    {
        if(pBlocks[times].block != nullptr &&
           pBlocks[times].is_none == false)
        {
            if(pBlocks[times].is_uncombined == true)
            {//障碍块刷新
                OneBlock = new block(en_empty); //障碍块图形构造
            }
            else
            {//数字块刷新
                bShape = numToShape(pBlocks[times].block->get_number());
                OneBlock = new block(bShape); //数字块图形构造
            }
            OneBlock->setPos((times/DE_BOARD_ROW)*DE_BLOCKLENGTH + DE_BOARD_XPOS,
                             (DE_BOARD_ROW - times%DE_BOARD_ROW - 1)*DE_BLOCKLENGTH + DE_BOARD_YPOS);    //方块位置设置
            addToGroup(OneBlock);
        }
    }
    qDebug()<<"paintBoard all block addToGroup finished!"<<endl;
    show();
}
