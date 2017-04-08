#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include "clientblock.h"
#include <QDateTime>
#include "attribute.h"
#include <Qtimer>
namespace Ui {
class Server;
}

class Server : public QWidget{
    Q_OBJECT
public slots:
    //void newClient();//有从机建立连接请求时，建立TCP连接
    //void updateUI();//更新面板上各从机的状态
public:
    explicit Server(QWidget *parent = 0);
    ~Server();
    void schedule();//1.遍历queue把服务完成的从机放到队尾；2.调用qSort对服务未完成的从机排序（选前三），依据优先级
    QTimer *timer = new QTimer(this);
    QDateTime Date=QDateTime::currentDateTime();
    QTime time=QTime::currentTime();
    int t=0,Year,Month,Day,Hour,Min,nextClientID=1;
    int clientID[5];
    Attribute attribute;
private slots:
    void on_check1_clicked();
    void on_check2_clicked();
    void on_check3_clicked();
    void on_check4_clicked();
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_cold_clicked();
    void on_hot_clicked();
public slots:
    void onTimeOut();
private:
    Ui::Server *ui;
    //QTcpServer _server;
    QList<ClientBlock* > _queue;//存四个从控机，最多前三个是正在被服务
};
#endif // SERVER_H
