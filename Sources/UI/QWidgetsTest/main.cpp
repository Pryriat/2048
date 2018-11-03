#include "public.h"
#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{    
    QApplication app(argc, argv);

        QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());

        // 设置随机数的初始值
        qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

        QPixmap pix(":/images/gameLoading.png");
        QSplashScreen splash(pix);
        splash.resize(pix.size());
        splash.show();

        //延时
    #if 1
        int delayTime = 5;
        QElapsedTimer timer;
        timer.start();
        while(timer.elapsed() < (delayTime * 150))
        {
             app.processEvents();
        }
    #endif

    #if 0
        QDateTime n=QDateTime::currentDateTime();
        QDateTime now;
        do{
            now=QDateTime::currentDateTime();
            app.processEvents();
        } while (n.secsTo(now)<=5); //5为需要延时的秒数
    #endif
        //app.processEvents();

        MainWindow w;
        w.show();
        splash.finish(&w);
        return app.exec();



}
