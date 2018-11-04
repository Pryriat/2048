#ifndef WIDGET_H
#define WIDGET_H

#include"qtredis.h"
#include <QWidget>
#include<QPushButton>
#include<QLabel>
#include<QLineEdit>
#include <QtNetwork/QNetworkInterface>
#include<QString>
#include<QMessageBox>
#include<QVBoxLayout>
#include<QGridLayout>
#include"database_management.h"
class Widget : public QWidget
{
    Q_OBJECT
    DBM* Demo = new DBM("127.0.0.1",6379);
    //控件声明与初始化
    QPushButton* Click = new QPushButton("Send");
    QPushButton* Result = new QPushButton("Get Result");
    QLabel* Machine_code = new QLabel();
    QLineEdit* Mark = new QLineEdit();
    QLineEdit* Authcode = new QLineEdit();
    QGridLayout* LineEdit_Layout = new QGridLayout();
    QVBoxLayout* MainLayout = new QVBoxLayout(this);
public:
    Widget(QWidget *parent = 0);
    ~Widget();
 public slots:
    void on_clicked();
    void show_result();
};

#endif // WIDGET_H
