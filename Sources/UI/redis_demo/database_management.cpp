#include"database_management.h"
#include<QMessageBox>

bool DBM::Start_Connection()
{
    if(!this->Nt.allInterfaces().length())//没有网络连接设备
    {
        qDebug()<<"No Network Interface Enabled!\n";//Debug窗口输出信息
        QMessageBox::critical(NULL,"Warning","No Network Interface!");
        return false;
    }

    if(!this->Connector.openConnection())//连接数据库失败
    {
        qDebug()<<"Connection failed!";
        QMessageBox::critical(NULL,"Warning","Connection Failed!");
        return false;
    }

    return true;
}


QString DBM::Query_on_Clicked()
{
    try
    {//hget格式：hget(属性名，键名)
        QString Mark = this->Connector.hget(this->Nt.allInterfaces()[0].hardwareAddress(),"Mark");
        qDebug()<<"Query Success!\n";
        return Mark;
    }
    catch(...)
    {
        qDebug()<<"Query Failed!\n";
        QMessageBox::critical(NULL,"Error","Query Operation Failed!");
        return "";//查询失败返回空字符串
    }
}

bool DBM::Upload_on_Clicked(QString Mark, QString AuthCode)//AuthCode类型可以后续讨论修改
{
    try
    {//hset格式：hset(属性名，键名，值）
        this->Connector.hset(this->Nt.allInterfaces()[0].hardwareAddress(),"Mark",Mark);
        this->Connector.hset(this->Nt.allInterfaces()[0].hardwareAddress(),"AuthCode",AuthCode);
        qDebug()<<"Upload Success!\n";
        return true;
    }
    catch(...)
    {
        qDebug()<<"Upload Failed!\n";
        QMessageBox::critical(NULL,"Error","Upload Failed!");
        return false;//上传失败返回false
    }
}


