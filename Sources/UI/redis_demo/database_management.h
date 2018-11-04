/*
 * Author: Chernobyl
 * Date: 2018/11/04
 * 用户界面分数上传与查询接口实现
*/

#ifndef DATABASE_MANAGEMENT_H
#define DATABASE_MANAGEMENT_H
#include"qtredis.h"
#include<QString>
#include <QtNetwork/QNetworkInterface>
class DBM
{
public:
    bool Start_Connection();//判断连接数据库是否成功
    bool Upload_on_Clicked(QString Mark, QString AuthCode);//上传分数
    QString Query_on_Clicked();//查询分数
    DBM(QString IP_Address, unsigned int port):Connector(IP_Address,port){};
private:
    QNetworkInterface Nt;
    QtRedis Connector;
};
#endif // DATABASE_MANAGEMENT_H
