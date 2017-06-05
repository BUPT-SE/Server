﻿#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QLabel>
#include <QDateTime>
#include <QTimer>
#include <QMessageBox>
#include "clientblock.h"
#include "room.h"
#include "database.h"
#include "configdialog.h"

#define ROOM0_PATH "room0.txt"
#define ROOM1_PATH "room1.txt"
#define ROOM2_PATH "room2.txt"
#define ROOM3_PATH "room3.txt"

namespace Ui {
    class Server;
}

class ClientBlock;

class Server : public QWidget{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = 0);
    ~Server();

private slots:
    void newConnection();
    void disConnection(ClientBlock *client);
    void onTimeOut();
    void updateUI(ClientBlock *client);//更新面板上各从机的状态
    void checkIsCheckedIn(ClientBlock *client);
    void setBill(ClientBlock *client);
    void setCnt();                      //设置cnt立即调度

    void on_check1_clicked();
    void on_check2_clicked();
    void on_check3_clicked();
    void on_check4_clicked();
    void on_onButton_clicked();
    void on_okButton_clicked();
    void on_offButton_clicked();
    void on_paylist1_clicked();
    void on_paylist2_clicked();
    void on_paylist3_clicked();
    void on_paylist4_clicked();
    void on_detail1_clicked();
    void on_detail2_clicked();
    void on_detail3_clicked();
    void on_detail4_clicked();
    void on_deleteRecordButton_clicked();


private:
    Ui::Server *ui;
    QTcpServer *_tcpServer;
    QList<ClientBlock *> _queue;//存四个从控机，最多前三个是正在被服务
    QTimer *_timer;
    QDateTime _date;
    QTime _time;
    int _nextClientID;
    int _clientID[4];
    void sortByPriority(QList<ClientBlock*> &queue);
    void schedule();//1.遍历queue把服务完成的从机放到队尾；2.调用qSort对服务未完成的从机排序（选前三），依据优先级
    int cnt;
    QVector<Room> rooms;
    QVector<Bill> bills;
};
#endif // SERVER_H
