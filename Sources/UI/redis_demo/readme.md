# 2048 ���ݿ�ӿ���ʾ��

## ����

�����뱾��/Զ��Redis���ݿ����Ӳ�������Ϸ�����Ľӿ����Ӧʵ��

## �ӿ�

```cpp
bool Start_Connection();//�ж��������ݿ��Ƿ�ɹ���������������ֻ�������һ��
bool Upload_on_Clicked(QString Mark, QString AuthCode);//�ϴ���������֤�룬�������쳣����
QString Query_on_Clicked();//��ѯ�������������쳣����
DBM(QString IP_Address, unsigned int port):Connector(IP_Address,port){};//����������һ����Ϊip���ڶ�����Ϊ���ݿ�˿ڣ�Ĭ��Ϊ6379
```

## �ļ���

```
�ӿ�
|--database_management.h
|--qtredis.h
|--reader.h
|--database_management.cpp
|--qtredis.cpp
|--reader.cpp
```

## ����

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
        //�ϴ��ɹ�
    }
    else
    {
        //�ϴ�ʧ��
    }
}

void Widget::show_result()
{
    QString Mark = this->Demo->Query_on_Clicked();
    if(Mark.length()>0)
        QMessageBox::about(this,"Mark:",Mark);
    else
    {
        //��ѯʧ��
    }
}

```

