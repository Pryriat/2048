#ifndef PUBLIC_H
#define PUBLIC_H

#include <atomic>
#include <QTextCodec>
#include <QTime>
#include <QSplashScreen>
#include <QDebug>
#include <QElapsedTimer>
#include <QDateTime>
#include <QGraphicsObject>
#include <QGraphicsItemGroup>
#include <QGraphicsView>
#include <QMainWindow>
#include <QObject>
#include <QKeyEvent>
#include <QTransform>
#include <QGraphicsItem>
#include <QTimer>
#include <QMultiMap>
#include <QtMultimedia/QMediaPlayer>
#include <QIcon>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSlider>
#include <QPropertyAnimation>
#include <QGraphicsBlurEffect>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QApplication>
#include <QLabel>
#include <QFileInfo>
#include <QDebug>

typedef enum blockShape{ en_empty,
                         en_2,
                         en_4,
                         en_8,
                         en_16,
                         en_32,
                         en_64,
                         en_128,
                         en_256,
                         en_512,
                         en_1024,
                         en_2048,
                         en_4096} blockShape;

//棋盘位置坐标
#define DE_BOARD_XPOS 80
#define DE_BOARD_YPOS 20

//下一块方块的坐标
#define DE_NEXT_XPOS 527
#define DE_NEXT_YPOS 38

//棋盘行列长度宏定义
#define DE_BOARD_ROW 7
#define DE_BOARD_COLUMN 5

//难度宏定义
#define DE_DIFFICULITY 1

//方块像素长度
#define DE_BLOCKLENGTH 60


#endif // PUBLIC_H
