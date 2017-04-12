#ifndef SERVER_H
#define SERVER_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include "clientblock.h"
#include <QDateTime>
#include <Qtimer>
namespace Ui {
class Server;
}

class Server : public QWidget
{
    Q_OBJECT
public slots:
    void newClient();//有从机建立连接请求时，建立TCP连接
    void updateUI();//更新面板上各从机的状态
public:
    explicit Server(QWidget *parent = 0);
    ~Server();
    void schedule();//1.遍历queue把服务完成的从机放到队尾；2.调用qSort对服务未完成的从机排序（选前三），依据优先级
    QTime time = QTime::currentTime();
    int hour=time.hour();
    int min=time.minute();

    QDateTime dateTime=QDateTime::currentDateTime();
    int year=dateTime.date().year();
    int month=dateTime.date().month();
    int day=dateTime.date().day();

private slots:
    void on_check1_clicked();
    void on_check2_clicked();
    void on_check3_clicked();
    void on_check4_clicked();
    void on_pushButton_clicked();
public slots:
    void timeout();
   // QDateTime dateTime=QDateTime::currentDateTime();
    //int year=dateTim
private:
    Ui::Server *ui;
    QTcpServer _server;
    QTimer *timer;
    QList<ClientBlock* > _queue;//存四个从控机，最多前三个是正在被服务，
};

#endif // SERVER_H
