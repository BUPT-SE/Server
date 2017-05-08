#ifndef CLIENTBLOCK_H
#define CLIENTBLOCK_H

#include <QTcpSocket>
#include <qmath.h>
#include "server.h"

#define SPD_LOW     0                   //风速低档
#define SPD_MID     1                   //风速中档
#define SPD_HIGH    2                   //风速高档
#define MODE_COOL   0                   //工作模式制冷
#define MODE_HEAT   1                   //工作模式制热
#define DELTA       0.1                 //温度变化间隔

class ClientBlock : public QObject
{
    Q_OBJECT
public slots:
    void readMessage();                 //判断请求是否合理，包括目标温度当前温度是否合理，若接收到关机请求，则发送shutdown信号
signals:
    void shutdown(ClientBlock*);        //当接收到从控机的关机请求时，此信号用于通知Server销毁自己
    void update(ClientBlock*);          //更新UI
    void isCheckedIn(ClientBlock*);     //发送信号看此房间是否check in
    
public:
    explicit ClientBlock(QTcpSocket* socket, double lowestTmp, double highestTmp,
                         double targetTmp, int mode, QObject *parent = 0);//根据socket和主机工作模式构造CB
    ~ClientBlock();
    int getPriority();                  //更新优先级，得到优先级，数字越小优先级越大
    void setID(int id);                 //设置客户ID
    void setIsCheckedIn(bool flag);     //设置是否check in
    int getRoomNum();                   //返回房间号
    double getRoomTmp();                //返回室温
    double getTargetTmp();              //返回目标温度
    int getWindSpeed();                 //返回风速
    double getFee();                    //返回费用
    double getKwh();                    //返回金额
    void check();                       //先判断是否服务完成，再判断是否被挂起（挂起不变），若不为0count--, 若count==0，变温度，再判断是否达到目标温度，改变isFinished,发消息，重制count，
    void setSysTime(QString time);      //设置系统时间
    bool isSatisfied();                 //是否服务完成
    bool isServed();                    //返回是否在被服务
    void setIsServed(bool isServed);    //设置是否被服务
    void sendMessage();                 //向从控机发送消息，包括变温，和被挂起

public:
    QTcpSocket* _socket;                //套接字

private:
    void updateCount();                 //count变为0时，或从机发送风速变化请求，需根据风速重新计算count
    QJsonObject toJson();               //把属性转化成Json格式
    void setFromJson(int flag, QByteArray byteArray);//从Json格式改变属性值
    void incDeltaRoomTmp();             //室温升0.1度
    void decDeltaRoomTmp();             //室温降0.1度
    void sendFirstMessage();

private:
    int _id;                            //客户id
    bool _isCheckedIn;                  //是否check in
    int _count;                         //下次发送变温消息的计时器
    double _tmpFee;                     //本次服务的费用
    double _tmpKwh;                     //本次服务的消耗能量
    int _suspended;                     //累计挂起的时间
    bool _isSatisfied;                  //服务完成(达到目标温度)为true
    QString _sysTime;                   //系统时间
    int _roomNum;                       //房间号
    int _windSpeed;                     //风速，低档，中档，高档
    double _roomTmp;                    //室温
    int _mode;                          //工作模式，制冷，制热
    double _targetTmp;                  //目标温度
    bool _power;                        //开关机
    bool _isServed;                     //是否正在被服务
    double _Kwh;                        //消耗的电量
    double _fee;                        //所需支付总的费用
    double _lowestTmp;                  //允许设置目标温度的最低温度
    double _highestTmp;                 //运行设置目标温度的最高温度

};

#endif // CLIENTBLOCK_H
