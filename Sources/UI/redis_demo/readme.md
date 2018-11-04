# 2048 数据库接口与示例

## 综述

用于与本地/远程Redis数据库连接并传输游戏分数的接口与对应实现

## 接口

```cpp
bool Start_Connection();//判断连接数据库是否成功，在整个程序中只允许调用一次
bool Upload_on_Clicked(QString Mark, QString AuthCode);//上传分数、认证码，包含简单异常处理
QString Query_on_Clicked();//查询分数，包含简单异常处理
DBM(QString IP_Address, unsigned int port):Connector(IP_Address,port){};//构造器，第一参数为ip，第二参数为数据库端口，默认为6379
```

## 文件树

```
接口
|--database_management.h
|--qtredis.h
|--reader.h
|--database_management.cpp
|--qtredis.cpp
|--reader.cpp
```

## 用例

```cpp
this->Demo->Start_Connection();

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

```

