#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include <QLabel>
#include <QDateTime>
#include <Qtimer>
#include "attribute.h"
#include "clientblock.h"
#include "room.h"
#include "database.h"
#include "configdialog.h"

namespace Ui {
    class Server;
}

class ClientBlock;

class Server : public QWidget{
    Q_OBJECT
public slots:

    //void newClient();//有从机建立连接请求时，建立TCP连接
    //void updateUI();//更新面板上各从机的状态
public:
    explicit Server(QWidget *parent = 0);
    ~Server();
    //void schedule();//1.遍历queue把服务完成的从机放到队尾；2.调用qSort对服务未完成的从机排序（选前三），依据优先级
    Attribute attribute;
    QVector<room> rooms;
private slots:
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
public slots:
    void onTimeOut();
private:
    Ui::Server *ui;
    QTcpServer _server;
    QList<ClientBlock *> _queue;//存四个从控机，最多前三个是正在被服务
    QTimer *timer;
    int t,Year,Month,Day,Hour,Min,nextClientID;
    int clientID[4];
};
#endif // SERVER_H
