#ifndef CLIENTBLOCK_H
#define CLIENTBLOCK_H

#include <QTcpSocket>
#include "attribute.h"

class ClientBlock : public QObject
{
    Q_OBJECT
public slots:
    void readMessage();             //判断请求是否合理，包括目标温度当前温度是否合理，若接收到关机请求，则发送shutdown信号
signals:
    void shutdown(ClientBlock* );        //当接收到从控机的关机请求时，此信号用于通知Sever销毁自己
    void update(QString roomNum);        //当收到消息时，触发此信号，主机面板上更新从机状态
public:
    explicit ClientBlock(QTcpSocket* socket, int mode, QObject *parent = 0);//根据socket和主机工作模式构造CB
    int getPriority();              //更新优先级，得到优先级
    void check();                   //先判断是否服务完成，再判断是否被挂起（挂起不变），若不为0count--, 若count==0，变温度，再判断是否达到目标温度，改变isFinished,发消息，重制count，
    void sendMessage();             //向从控机发送消息，包括变温，和被挂起
    bool isSatisfied();             //是否服务完成
    void setMode();                 //设置从机工作模式
    void setDefRoomTmp();           //设置从机缺省室温

private:
    void updateCount();             //count变为0时，或从机发送风速变化请求，需根据风速重新计算count
private:
    Attribute _attribute;           //从控机
    QTcpSocket* _socket;            //套接字
    int _count;                     //下次发送变温消息的计时器
    int _suspended;                 //累计挂起的时间
    bool _satisfied;                //服务完成(达到目标温度)为true
};

#endif // CLIENTBLOCK_H


//从控机可以修改的属性：房间号，目标温度，室温（挂起时的室温变化），风速，开机关机
//主机可以修改的属性：工作模式，室温（被服务时的室温变化），是否被挂起
