#include "board.h"
#include "mainwindow.h"
#include "framework_base.h"
#include "public.h"

// 游戏的初始速度
static const qreal INITSPEED = 1000;

static const QString SOUNDPATH = "./sounds/";

MainWindow::MainWindow(QWidget *parent) :
    QGraphicsView (parent)
{
    //ui->setupUi(this);
    initView();
}

MainWindow::~MainWindow()
{
    //delete ui;
}

// 初始化游戏界面
void MainWindow::initView()
{
    // 使用抗锯齿渲染
    setRenderHint(QPainter::Antialiasing);

    // 设置缓存背景，这样可以加快渲染速度
    setCacheMode(CacheBackground);
    setWindowTitle(QString::fromLocal8Bit("My2048"));
    setWindowIcon(QIcon(":/images/gameIcon.png"));
    setMinimumSize(660, 460);
    setMaximumSize(660, 460);

    // 设置场景
    QGraphicsScene *scene = new QGraphicsScene;
    scene->setSceneRect(0, 0, 654, 453);
    scene->setBackgroundBrush(QPixmap(":/images/gameMenu.png"));
    setScene(scene);

    //原框架中画线算法，暂不需要
    /*
    // 方块可移动区域的四条边界线
    topLine = scene->addLine(197, 47, 403, 47);
    bottomLine = scene->addLine(197,453,403,453);
    leftLine = scene->addLine(197,47,197,453);
    rightLine = scene->addLine(403,47,403,453);
    */

    // 当前方块和提示方块
    pBoard = new board();   //当前方块
    connect(pBoard, SIGNAL(boardChange()), this, SLOT(end_judge()));
    connect(pBoard, SIGNAL(gameFinished()), this, SLOT(gameOver()));
    connect(pBoard->getTimer(), SIGNAL(timeout()), this, SLOT(blockDrop()));   //将计时器信号与自由下落函数连接，用于驱动方块自由下落
    scene->addItem(pBoard);
    nextBoard = new board();    //提示方块
    scene->addItem(nextBoard);

    // 得分文本
    gameScoreText = new QGraphicsTextItem();
    scene->addItem(gameScoreText);
    gameScoreText->setFont(QFont("幼圆", 10, QFont::Bold));
    gameScoreText->setPos(400, 20);

    // 级别文本
    gameLevelText = new QGraphicsTextItem();
    scene->addItem(gameLevelText);
    gameLevelText->setFont(QFont("幼圆", 30, QFont::Bold));
    gameLevelText->setPos(20, 150);

    //原框架中的画线代码，暂不需要
    /*
    // 设置初始为隐藏状态
    topLine->hide();
    bottomLine->hide();
    leftLine->hide();
    rightLine->hide();
    gameLevelText->hide();
    */

    gameScoreText->hide();

    // 黑色遮罩
    QWidget *mask = new QWidget;
    mask->setAutoFillBackground(true);
    mask->setPalette(QPalette(QColor(0, 0, 0, 150)));
    mask->resize(660,460);
    maskWidget = scene->addWidget(mask);
    maskWidget->setPos(0, 0);
    // 设置其Z值为1，这样可以处于Z值为0的图形项上面
    maskWidget->setZValue(1);
    maskWidget->hide();


    // 选项面板
    QWidget *option = new QWidget;


    QPushButton *optionCloseButton = new QPushButton(QString::fromLocal8Bit("CLOSE"), option);
    QPalette palette;
    palette.setColor(QPalette::ButtonText, Qt::black);
    optionCloseButton->setPalette(palette);
    optionCloseButton->move(120, 300);
    connect(optionCloseButton, SIGNAL(clicked()), option, SLOT(hide()));
    option->setAutoFillBackground(true);
    option->setPalette(QPalette(QColor(0, 0, 0, 50)));
    option->resize(300, 400);
    QGraphicsWidget *optionWidget = scene->addWidget(option);
    optionWidget->setPos(250, 50);
    optionWidget->setZValue(3);
    optionWidget->hide();

    // 帮助面板
    QWidget *help = new QWidget;
    QPushButton *helpCloseButton = new QPushButton(QString::fromLocal8Bit("CLOSE"), help);
    helpCloseButton->setPalette(palette);
    helpCloseButton->move(120, 300);
    connect(helpCloseButton, SIGNAL(clicked()), help, SLOT(hide()));
    help->setAutoFillBackground(true);
    help->setPalette(QPalette(QColor(0, 0, 0, 50)));
    help->resize(300, 400);
    QGraphicsWidget *helpWidget = scene->addWidget(help);
    helpWidget->setPos(150, 50);
    helpWidget->setZValue(3);
    helpWidget->hide();

    QLabel *helpLabel = new QLabel(help);
    helpLabel->setText(QString::fromLocal8Bit("<h1><font color=white>@Enjoy!"
                             "<br>Here is the help text</font></h1>"));
    helpLabel->setAlignment(Qt::AlignCenter);
    helpLabel->move(30, 150);


    // 计分板面板
    QWidget *scoreBoard = new QWidget;
    QPushButton *scoreBoardCloseButton = new QPushButton(QString::fromLocal8Bit("CLOSE"), scoreBoard);
    scoreBoard->setPalette(palette);
    scoreBoard->move(20,300);
    connect(scoreBoardCloseButton,SIGNAL(clicked()), scoreBoard, SLOT(hide()));
    scoreBoard->setAutoFillBackground(true);
    scoreBoard->setPalette(QPalette(QColor(0,0,0,50)));
    scoreBoard->resize(500,400);
    QGraphicsWidget *scoreBoardWidget = scene->addWidget(scoreBoard);
    scoreBoardWidget->setPos(50,50);
    scoreBoardWidget->setZValue(3);
    scoreBoardWidget->hide();

    QLabel *scoreBoardLabel = new QLabel(scoreBoard);
    scoreBoardLabel->setText(QString::fromLocal8Bit("<h1><font color=white>@Enjoy!"
                                                    "<br>Here is the ScoreBoard text</font></h1>"));
    scoreBoardLabel->setAlignment(Qt::AlignCenter);
    scoreBoardLabel->move(30,150);

    // 声明文本
    gameWelcomeText = new QGraphicsTextItem();
    scene->addItem(gameWelcomeText);
    gameWelcomeText->setHtml(QString::fromLocal8Bit("<font color=black>C++轮子队出品</font>"));
    gameWelcomeText->setFont(QFont("幼圆", 5, QFont::Normal));   //QFont("Times", 30, QFont::Bold)
    gameWelcomeText->setPos(20, 430);
    gameWelcomeText->setZValue(2);


    // 游戏暂停文本
    gamePausedText = new QGraphicsTextItem();
    scene->addItem(gamePausedText);
    gamePausedText->setHtml(QString::fromLocal8Bit("<font color=red>PAUSING</font>"));
    gamePausedText->setFont(QFont("幼圆", 15, QFont::Bold));
    gamePausedText->setPos(171, 100);
    gamePausedText->setZValue(2);
    gamePausedText->hide();


    // 游戏结束文本
    gameOverText = new QGraphicsTextItem();
    scene->addItem(gameOverText);
    gameOverText->setHtml(QString::fromLocal8Bit("<font color=white>GameOver</font>"));
    gameOverText->setFont(QFont("幼圆", 30, QFont::Bold));
    gameOverText->setPos(197, 100);
    gameOverText->setZValue(2);
    gameOverText->hide();



    // 游戏中使用的按钮
    QPushButton *button1 = new QPushButton(QString::fromLocal8Bit("START"));
    QPushButton *button2 = new QPushButton(QString::fromLocal8Bit("OPTION"));
    QPushButton *button3 = new QPushButton(QString::fromLocal8Bit("HELP"));
    QPushButton *button4 = new QPushButton(QString::fromLocal8Bit("EXIT"));
    QPushButton *button5 = new QPushButton(QString::fromLocal8Bit("RESTART"));
    QPushButton *button6 = new QPushButton(QString::fromLocal8Bit("PAUSE"));
    QPushButton *button7 = new QPushButton(QString::fromLocal8Bit("MENU"));
    QPushButton *button8 = new QPushButton(QString::fromLocal8Bit("RETURN"));
    QPushButton *button9 = new QPushButton(QString::fromLocal8Bit("EXIT"));
    QPushButton *button10 = new QPushButton(QString ::fromLocal8Bit("SCOREBOARD"));


    connect(button1, SIGNAL(clicked()), this, SLOT(startGame()));
    connect(button2, SIGNAL(clicked()), option, SLOT(show()));
    connect(button3, SIGNAL(clicked()), help, SLOT(show()));
    connect(button4, SIGNAL(clicked()), qApp, SLOT(quit()));
    connect(button5, SIGNAL(clicked()), this, SLOT(restartGame()));
    connect(button6, SIGNAL(clicked()), this, SLOT(pauseGame()));
    connect(button7, SIGNAL(clicked()), this, SLOT(gameExit()));
    connect(button8, SIGNAL(clicked()), this, SLOT(returnGame()));
    connect(button9, SIGNAL(clicked()), this, SLOT(gameExit()));
    connect(button10,SIGNAL(clicked()), scoreBoard, SLOT(show()));

    startButton = scene->addWidget(button1);
    optionButton = scene->addWidget(button2);
    helpButton = scene->addWidget(button3);
    exitButton = scene->addWidget(button4);
    restartButton = scene->addWidget(button5);
    pauseButton = scene->addWidget(button6);
    showMenuButton = scene->addWidget(button7);
    returnButton = scene->addWidget(button8);
    finishButton = scene->addWidget(button9);
    scoreBoardButton = scene->addWidget(button10);

    scoreBoardButton->setPos(20,400);
    startButton->setPos(550, 200);
    optionButton->setPos(550, 250);
    helpButton->setPos(550, 300);
    exitButton->setPos(550, 350);
    restartButton->setPos(510, 150);
    pauseButton->setPos(510, 200);
    showMenuButton->setPos(510, 250);
    returnButton->setPos(185, 200);
    finishButton->setPos(370, 250);

    startButton->setZValue(2);
    scoreBoardButton->setZValue(2);
    optionButton->setZValue(2);
    helpButton->setZValue(2);
    exitButton->setZValue(2);
    restartButton->setZValue(2);
    returnButton->setZValue(2);
    finishButton->setZValue(2);

    restartButton->hide();
    finishButton->hide();
    pauseButton->hide();
    showMenuButton->hide();
    returnButton->hide();



    backgroundMusic=new QMediaPlayer();
    connect(backgroundMusic, SIGNAL(positionChanged(qint64)), this, SLOT(positionChanged(qint64)));
    clearRowSound=new QMediaPlayer();
    backgroundMusic->setVolume(60);
    clearRowSound->setVolume(60);

    backgroundMusic->setMedia(QUrl::fromLocalFile(SOUNDPATH + "background.mp3"));
    clearRowSound->setMedia(QUrl::fromLocalFile(SOUNDPATH + "clearRow.mp3"));
    backgroundMusic->play();

    QLabel *volumeLabel=new QLabel(QString::fromLocal8Bit("Music:"));
    QSlider *volume1=new QSlider(Qt::Horizontal);
    volume1->setRange(60,100);
    QBoxLayout *Layout1 = new QHBoxLayout();
    Layout1->addWidget(volumeLabel);
    Layout1->addWidget(volume1);
    connect(volume1,SIGNAL(valueChanged(int)),backgroundMusic,SLOT(setVolume(int)));

    QLabel *volumeLabe2=new QLabel(QString::fromLocal8Bit("Sound:"));
    QSlider *volume2=new QSlider(Qt::Horizontal);
    volume2->setRange(60,100);
    QBoxLayout *Layout2 = new QHBoxLayout();
    Layout2->addWidget(volumeLabe2);
    Layout2->addWidget(volume2);
    connect(volume2,SIGNAL(valueChanged(int)),clearRowSound,SLOT(setVolume(int)));


    QBoxLayout *popupLayout = new QVBoxLayout();
    popupLayout->addLayout(Layout2);
    popupLayout->addLayout(Layout1);
    option->setLayout(popupLayout);
    //startGame();
}

// 初始化游戏
void MainWindow::initGame()
{
    qDebug()<<"Func:initGame hits!"<<endl;

    /*
    pBoard->createBlock();
    pBoard->setFocus();
    pBoard->startTimer(INITSPEED);
    gameSpeed = INITSPEED;
    nextBoard->createBlock();   //下一个方块的位置
    scene()->setBackgroundBrush(QPixmap(":/images/gameBackground.png"));
    */

    //显示文字代码段
    /*
    gameLevelText->setHtml(QString::fromLocal8Bit("<font color=white>No.1<</font>"));
     //tr()换成QString::fromLocal8Bit显示中文
    */

    //原框架中的画线代码，暂不需要
    /*
    topLine->show();
    bottomLine->show();
    leftLine->show();
    rightLine->show();
    */

    pBoard->startTimer(INITSPEED);
    gameSpeed = INITSPEED;
    scene()->setBackgroundBrush(QPixmap(":/images/gameBackground.png"));
    gameScoreText->setHtml(QString::fromLocal8Bit("<font color=red>0</font>"));
    restartButton->show();
    pauseButton->show();
    showMenuButton->show();
    gameScoreText->show();
    gameLevelText->show();

    //棋盘类及框架类初始化
    framework *ptr_framework = pBoard->getFrameworkPtr();
    framework_block_item *pBlock = pBoard->getFrameworkPtr()->current_status();
    for(int x = 0; x < 5; ++x)
    {
        for(int y = 0; y < 7; ++y)
        {
            pBlock->block = nullptr;
            pBlock->is_none = true;
            pBlock->is_uncombined = false;
            pBlock++;
        }
    }
    ptr_framework->setMovingBlock(ptr_framework->generate_block());
    pBoard->paintBoard(QPointF(DE_BOARD_XPOS,DE_BOARD_YPOS));

    backgroundMusic->setMedia(QUrl::fromLocalFile(SOUNDPATH + "background01.mp3"));
    backgroundMusic->play();

}

// 开始游戏
void MainWindow::startGame()
{
    qDebug()<<"Func:startGame hits!"<<endl;
    gameWelcomeText->hide();
    startButton->hide();
    scoreBoardButton->hide();
    optionButton->hide();
    helpButton->hide();
    exitButton->hide();
    maskWidget->hide();

    initGame();
}

// 清空满行
bool MainWindow::end_judge()
{
    qDebug()<<"Func:end_judge hits!"<<endl;

    /*
    // 获取比一行方格较大的矩形中包含的所有小方块
        for (int y = 429; y > 50; y -= 20) {
            Qt::SortOrder order;
            QList<QGraphicsItem *> list =scene()->items(199, y, 202, 22, Qt::ContainsItemShape, order,QTransform());
            //QList<QGraphicsItem *> list = scene()->items(199, y, 202, 22, Qt::ContainsItemShape);
            // 如果该行已满
            if (list.count() == 10) {
                foreach (QGraphicsItem *item, list) {
                    block *pBlock = (block *) item;

                    //动画效果
                    QGraphicsBlurEffect *blurEffect = new QGraphicsBlurEffect;
                    pBlock->setGraphicsEffect(blurEffect);
                    QPropertyAnimation *animation = new QPropertyAnimation(pBlock, "scale");
                    animation->setEasingCurve(QEasingCurve::OutBounce);
                    animation->setDuration(250);
                    animation->setStartValue(4);
                    animation->setEndValue(0.25);
                    animation->start(QAbstractAnimation::DeleteWhenStopped);
                    connect(animation, SIGNAL(finished()), pBlock, SLOT(deleteLater()));
                    //box->deleteLater();
                }
                // 保存满行的位置
                rows << y;
            }
        }
        // 如果有满行，下移满行上面的各行再出现新的方块组
        // 如果没有满行，则直接出现新的方块组
        if(rows.count() > 0) {
            clearRowSound->play();
            moveBox();
        } else {
            pBoard->createBlock(nextBoard->getBlockShape());
            // 清空并销毁提示方块组中的所有小方块
            nextBoard->clearBlock();
            nextBoard->createBlock();
        }
    */

    framework *pFramework = pBoard->getFrameworkPtr();
    framework_block_item *pBlock = pFramework->current_status();

    unsigned int x = pFramework->moving_block->get_x();
    unsigned int y = pFramework->moving_block->get_y();

    updateScore();  //更新得分

    if(0 == y)
    {
        pFramework->merge();    //触底即调用合并方法
        pFramework->is_generate();  //移动方块触底，进入新方块生成函数
    }

    else if(!pBlock[x * DE_BOARD_ROW + (y - 1)].is_none)
    {
        pFramework->merge();
        if(!pFramework->is_generate())
        {
            pFramework->is_end = true;
            emit pBoard->gameFinished();    //激发游戏结束信号
            return true;
        }
    }
    pBoard->paintBoard(QPointF(DE_BOARD_XPOS,DE_BOARD_YPOS));
    return false;

}

// 下移满行上面的所有小方块
void MainWindow::blockDrop()
{
    //qDebug()<<"Func:blockDrop hits!"<<endl;
    framework *pFramework = pBoard->getFrameworkPtr();
    framework_block_item *pBlocks = pFramework->current_status();

    unsigned int current_x = pFramework->moving_block->get_x();
    unsigned int current_y = pFramework->moving_block->get_y();

    qDebug()<<"Func:blockDrop hit!"<<endl;

    if(pFramework->is_end == true)
    {//此局游戏结束
        return;
    }

    if( 0 == current_y                                   //若触底则不执行
        || !pFramework->moving_block->get_is_moving())   //移动块因某种原因处于未移动状态，则下落不执行
    {
        qDebug()<<"current_x: "<<current_x<<endl;
        qDebug()<<"current_y: "<<current_y<<endl;
        qDebug()<<"!is_moving():"<<!pFramework->moving_block->get_is_moving()<<endl;
        qDebug()<<"Func:blockDrop() error hits!"<<endl;
        return ;

    }

    else if(!pBlocks[current_x * DE_BOARD_ROW + (current_y - 1)].is_none)//方块下接触到其他方块，则下落不执行
    {
        emit pBoard->boardChange();
        return;
    }

    else
    {
        //正常下落：
        pFramework->moving_block->modify_y(current_y - 1);
        pBlocks[current_x * DE_BOARD_ROW + current_y].is_none = true;
        pBlocks[current_x * DE_BOARD_ROW + current_y].block = nullptr;
        pBlocks[current_x * DE_BOARD_ROW + (current_y - 1)].is_none = false;
        pBlocks[current_x * DE_BOARD_ROW + (current_y - 1)].is_uncombined = false;
        pBlocks[current_x * DE_BOARD_ROW + (current_y - 1)].block = pFramework->moving_block;

        //qDebug()<<"Func:blockDrop() normal drop hits!"<<endl;
    }


    //画图
    pBoard->paintBoard(QPoint(DE_BOARD_XPOS,DE_BOARD_YPOS));

    emit pBoard->boardChange();

}

//更新分数并根据分数更换背景图或添加其他操作
void MainWindow::updateScore()
{ 
    qDebug()<<"Func:updateScore hits!"<<endl;

    framework *pFramework = pBoard->getFrameworkPtr();
    unsigned int score = pFramework->return_mark();
    unsigned int currentScore = gameScoreText->toPlainText().toUInt();
    currentScore = score;
    gameScoreText->setHtml(QString::fromLocal8Bit("<font color=red>%1</font>").arg(currentScore));

    if (currentScore < 500)
    {   //积分于0-500操作

    }
    else if (currentScore < 1000)
    {   //积分于500-1000操作
        /*
        //原框架此处为壁纸更换操作，现注释取消
        gameLevelText->setHtml(QString::fromLocal8Bit("<font color=white>No.2</font>"));
        scene()->setBackgroundBrush(QPixmap(":/images/background02.png"));
        gameSpeed = 300;
        pBoard->stopTimer();
        pBoard->startTimer(gameSpeed);
        */
    }
    else
    {
        // 添加下一个级别的设置
    }
}


void MainWindow::gameOver()
{
    qDebug()<<"Func:gameOver hits!"<<endl;

    // 游戏结束
    pauseButton->hide();
    showMenuButton->hide();
    maskWidget->show();
    gameOverText->show();
    restartButton->setPos(280, 200);

    finishButton->setPos(280,250);
    finishButton->show();


    pBoard->clearBlock();
    pBoard->hide();
    //框架类数组置空
    framework_block_item *pBlock = pBoard->getFrameworkPtr()->current_status();
    for(int times = 0; times < 5*7; ++times)
    {
        if(pBlock[times].block != nullptr)
        {
            delete pBlock[times].block;
            pBlock[times].block = nullptr;
         }
    }

    // 此处添加计分板相关函数

    pBoard->getFrameworkPtr()->setMarkZero();   //分数置0
    pBoard->getFrameworkPtr()->is_end = false;
}

// 重新开始游戏
void MainWindow::restartGame()
{
    qDebug()<<"Func:restartGame hits!"<<endl;

    maskWidget->hide();
    gameOverText->hide();
    finishButton->hide();
    restartButton->setPos(510, 150);

    // 销毁提示方块组和当前方块移动区域中的所有方块
    //nextBoard->clearBlock();
    pBoard->clearBlock();
    pBoard->hide();

    /*
    Qt::SortOrder order;
    foreach (QGraphicsItem *item, scene()->items(199, 49, 202, 402, Qt::ContainsItemShape,order,QTransform()))
    {
        // 先从场景中移除小方块，因为使用deleteLater()是在返回主事件循环后才销毁
        // 小方块的，为了在出现新的方块组时不发生碰撞，所以需要先从场景中移除小方块
        scene()->removeItem(item);
        block *box = (block *) item;
        box->deleteLater();
    }
    */

    //框架类数组置空
    framework_block_item *pBlock = pBoard->getFrameworkPtr()->current_status();
    for(int times = 0; times < 5*7; ++times)
    {
        if(pBlock[times].block != nullptr)
        {
            delete pBlock[times].block;
            pBlock[times].block = nullptr;
         }
    }
    pBoard->getFrameworkPtr()->setMarkZero();   //分数置0
    pBoard->getFrameworkPtr()->is_end = false;  //结束标志位置为false
    initGame();
}

void MainWindow::gameExit()
{
    qDebug()<<"Func:gameExit hits!"<<endl;

    gameOverText->hide();
    finishButton->hide();
    restartButton->setPos(510, 150);
    restartButton->hide();
    pauseButton->hide();
    showMenuButton->hide();
    gameScoreText->hide();
    gameLevelText->hide();
    maskWidget->hide();

    //原框架中的画线代码，暂不需要
    /*
    topLine->hide();
    bottomLine->hide();
    leftLine->hide();
    rightLine->hide();
    */

    /*
    nextBoard->clearBlock();
    pBoard->clearBlock();
    pBoard->hide();

    Qt::SortOrder order;
    foreach (QGraphicsItem *item, scene()->items(199, 49, 202, 402, Qt::ContainsItemShape,order,QTransform())) {
        block *pblock = (block *) item;
        pblock->deleteLater();
    }
    */

    pBoard->clearBlock();
    pBoard->hide();

    // 在进行游戏时按下“主菜单”按钮
    maskWidget->show();
    gameWelcomeText->show();
    startButton->show();
    scoreBoardButton->show();
    optionButton->show();
    helpButton->show();
    exitButton->show();
    scene()->setBackgroundBrush(QPixmap(":/images/gameMenu.png"));
}

// 暂停游戏
void MainWindow::pauseGame()
{
    qDebug()<<"Func:pauseGame hits!"<<endl;

    pBoard->stopTimer();
    restartButton->hide();
    pauseButton->hide();
    showMenuButton->hide();
    maskWidget->show();
    gamePausedText->show();
    returnButton->show();
}

// 返回游戏，处于暂停状态时
void MainWindow::returnGame()
{
    qDebug()<<"Func:returnGame hits!"<<endl;

    returnButton->hide();
    gamePausedText->hide();
    maskWidget->hide();
    restartButton->show();
    pauseButton->show();
    showMenuButton->show();
    pBoard->startTimer(gameSpeed);
}


// 如果正在进行游戏，当键盘按下时总是方块组获得焦点
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    //qDebug()<<"Func:MainWindow::keyPressEvent hits!"<<endl;
    if (pauseButton->isVisible())
        pBoard->setFocus();
    else
        pBoard->clearFocus();

    pBoard->keyPressEvent(event);
}


