#include "widget.h"
void on_clicked(QtRedis& redis, qint64 mark);

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    //布局初始化
    LineEdit_Layout->addWidget(Machine_code,0,0);
    LineEdit_Layout->addWidget(Mark,0,1);
    LineEdit_Layout->addWidget(Authcode,0,2);
    MainLayout->addLayout(LineEdit_Layout);
    MainLayout->addWidget(Click);
    MainLayout->addWidget(Result);

    //信号初始化
    QObject::connect(Click, SIGNAL(clicked(bool)), this, SLOT(on_clicked()));
    QObject::connect(Result, SIGNAL(clicked(bool)), this, SLOT(show_result()));

    //启动前初始化
    if(!this->Nt->allInterfaces().length())
    {
        qDebug()<<"No Network Interface Enabled!\n";
        Click->setEnabled(false);
    }
    else
    {
        qDebug()<<"Network Interface Found!\n";
        Machine_code->setText(this->Nt->allInterfaces()[0].hardwareAddress());
    }

    if(!this->redis->openConnection())
    {
        qDebug()<<"Connection failed!";
        QMessageBox::critical(this,"Warning","Connection Failed!");
        Click->setEnabled(false);
    }
}

void Widget::on_clicked()
{
    if(!this->Mark->text().length())
    {
        qDebug()<<"No Mark Input!";
        QMessageBox::critical(this,"Warning","Mark Input Failed!");
        return;
    }
    redis->hset(this->Nt->allInterfaces()[0].hardwareAddress(),"Mark",this->Mark->text());
    redis->hset(this->Nt->allInterfaces()[0].hardwareAddress(),"AuthCode",this->Authcode->text());
    QMessageBox::about(this,"Success","Upload Success!");
}

void Widget::show_result()
{
    QString Mark = redis->hget(this->Nt->allInterfaces()[0].hardwareAddress(),"Mark");
    QMessageBox::about(this,"Mark:",Mark);
}

Widget::~Widget()
{

}
