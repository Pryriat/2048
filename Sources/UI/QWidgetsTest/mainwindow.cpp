#include "mainwindow.h"
#include "public.h"

// 游戏的初始速度
static const qreal INITSPEED = 500;

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

    // 当前方块组和提示方块组
    pBoard = new board();
    connect(pBoard, SIGNAL(needNewBox()), this, SLOT(clearFullRows()));
    connect(pBoard, SIGNAL(gameFinished()), this, SLOT(gameOver()));
    scene->addItem(pBoard);
    nextBoard = new board();
    scene->addItem(nextBoard);

    // 得分文本
    gameScoreText = new QGraphicsTextItem();
    scene->addItem(gameScoreText);
    gameScoreText->setFont(QFont("Times", 10, QFont::Bold));
    gameScoreText->setPos(400, 20);

    // 级别文本
    gameLevelText = new QGraphicsTextItem();
    scene->addItem(gameLevelText);
    gameLevelText->setFont(QFont("Times", 30, QFont::Bold));
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
    mask->setPalette(QPalette(QColor(0, 0, 0, 0)));
    mask->resize(700, 500);
    maskWidget = scene->addWidget(mask);
    maskWidget->setPos(-50, -50);
    // 设置其Z值为1，这样可以处于Z值为0的图形项上面
    maskWidget->setZValue(1);


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
    gameWelcomeText->setFont(QFont("Consolas", 5, QFont::Normal));   //QFont("Times", 30, QFont::Bold)
    gameWelcomeText->setPos(20, 430);
    gameWelcomeText->setZValue(2);


    // 游戏暂停文本
    gamePausedText = new QGraphicsTextItem();
    scene->addItem(gamePausedText);
    gamePausedText->setHtml(QString::fromLocal8Bit("<font color=red>PAUSING</font>"));
    gamePausedText->setFont(QFont("Times", 15, QFont::Bold));
    gamePausedText->setPos(171, 100);
    gamePausedText->setZValue(2);
    gamePausedText->hide();


    // 游戏结束文本
    gameOverText = new QGraphicsTextItem();
    scene->addItem(gameOverText);
    gameOverText->setHtml(QString::fromLocal8Bit("<font color=white>GameOver</font>"));
    gameOverText->setFont(QFont("Times", 30, QFont::Bold));
    gameOverText->setPos(320, 100);
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
    connect(button7, SIGNAL(clicked()), this, SLOT(finishGame()));
    connect(button8, SIGNAL(clicked()), this, SLOT(returnGame()));
    connect(button9, SIGNAL(clicked()), this, SLOT(finishGame()));
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
    pBoard->createBlock(QPointF(300, 70));
    pBoard->setFocus();
    pBoard->startTimer(INITSPEED);
    gameSpeed = INITSPEED;
    nextBoard->createBlock(QPointF(500, 70));
    scene()->setBackgroundBrush(QPixmap(":/images/gameBackground.png"));

    //显示文字代码段
    /*
    gameLevelText->setHtml(QString::fromLocal8Bit("<font color=white>No.1<</font>"));
     //tr()换成QString::fromLocal8Bit显示中文
    */

    gameScoreText->setHtml(QString::fromLocal8Bit("<font color=red>0</font>"));

    restartButton->show();
    pauseButton->show();
    showMenuButton->show();
    gameScoreText->show();
    gameLevelText->show();

    //原框架中的画线代码，暂不需要
    /*
    topLine->show();
    bottomLine->show();
    leftLine->show();
    rightLine->show();
    */

    pBoard->show();

    backgroundMusic->setMedia(QUrl::fromLocalFile(SOUNDPATH + "background01.mp3"));

    backgroundMusic->play();

}

// 开始游戏
void MainWindow::startGame()
{
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
void MainWindow::clearFullRows()
{
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
            pBoard->createBlock(QPointF(300, 70), nextBoard->getBlockShape());
            // 清空并销毁提示方块组中的所有小方块
            nextBoard->clearBlock();
            nextBoard->createBlock(QPointF(500, 70));
        }
}

// 下移满行上面的所有小方块
void MainWindow::moveBox()
{
    // 从位置最靠上的满行开始
    for (int i = rows.count(); i > 0;  --i) {
        int row = rows.at(i - 1);
        Qt::SortOrder order;
        foreach (QGraphicsItem *item, scene()->items(199, 49, 202, row - 47, Qt::ContainsItemShape,order,QTransform()))
        {
            item->moveBy(0, 20);
        }
    }
    // 更新分数
    updateScore(rows.count());
    // 将满行列表清空为0
    rows.clear();
    // 等所有行下移以后再出现新的方块组
    pBoard->createBlock(QPointF(300, 70), nextBoard->getBlockShape());
    nextBoard->clearBlock();
    nextBoard->createBlock(QPointF(500, 70));
}

//更新分数并根据分数更换背景图或添加其他操作
void MainWindow::updateScore(const int fullRowNum)
{
    int score = fullRowNum * 100;
    int currentScore = gameScoreText->toPlainText().toInt();
    currentScore += score;
    gameScoreText->setHtml(QString::fromLocal8Bit("<font color=red>%1</font>").arg(currentScore));
    if (currentScore < 500)
    {

    }
    else if (currentScore < 1000)
    {
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
    // 游戏结束
    pauseButton->hide();
    showMenuButton->hide();
    maskWidget->show();
    gameOverText->show();
    restartButton->setPos(370, 200);
    finishButton->show();
}

// 重新开始游戏
void MainWindow::restartGame()
{
    maskWidget->hide();
    gameOverText->hide();
    finishButton->hide();
    restartButton->setPos(510, 150);

    // 销毁提示方块组和当前方块移动区域中的所有方块
    nextBoard->clearBlock();
    pBoard->clearBlock();
    pBoard->hide();
    Qt::SortOrder order;
    foreach (QGraphicsItem *item, scene()->items(199, 49, 202, 402, Qt::ContainsItemShape,order,QTransform()))
    {
        // 先从场景中移除小方块，因为使用deleteLater()是在返回主事件循环后才销毁
        // 小方块的，为了在出现新的方块组时不发生碰撞，所以需要先从场景中移除小方块
        scene()->removeItem(item);
        block *box = (block *) item;
        box->deleteLater();
    }

    initGame();
}

void MainWindow::finishGame()
{
    gameOverText->hide();
    finishButton->hide();
    restartButton->setPos(510, 150);
    restartButton->hide();
    pauseButton->hide();
    showMenuButton->hide();
    gameScoreText->hide();
    gameLevelText->hide();

    //原框架中的画线代码，暂不需要
    /*
    topLine->hide();
    bottomLine->hide();
    leftLine->hide();
    rightLine->hide();
    */

    nextBoard->clearBlock();
    pBoard->clearBlock();
    pBoard->hide();

    Qt::SortOrder order;
    foreach (QGraphicsItem *item, scene()->items(199, 49, 202, 402, Qt::ContainsItemShape,order,QTransform())) {
        block *pblock = (block *) item;
        pblock->deleteLater();
    }

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
    if (pauseButton->isVisible())
        pBoard->setFocus();
    else
        pBoard->clearFocus();
    QGraphicsView::keyPressEvent(event);
}


