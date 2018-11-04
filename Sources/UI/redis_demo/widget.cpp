#include "widget.h"
#include"database_management.h"

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
    this->Demo->Start_Connection();
}

void Widget::on_clicked()
{
    if(!this->Mark->text().length())
    {
        qDebug()<<"No Mark Input!";
        QMessageBox::critical(this,"Warning","Mark Input Failed!");
        return;
    }
    if(this->Demo->Upload_on_Clicked(this->Mark->text(),this->Authcode->text()))
    {
        //上传成功
    }
    else
    {
        //上传失败
    }
}

void Widget::show_result()
{
    QString Mark = this->Demo->Query_on_Clicked();
    if(Mark.length()>0)
        QMessageBox::about(this,"Mark:",Mark);
    else
    {
        //查询失败
    }
}

Widget::~Widget()
{

}
