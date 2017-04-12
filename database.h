#ifndef DATABASE_H
#define DATABASE_H

#include <QtSql>
#include <QString>
#include <QDebug>
#include <QDateTime>
#include "configdialog.h"

using namespace std;

struct operation
{
    int roomId;         //房间号
    int customerId;     //客户号
    QString time;       //时间 格式为
    int oper;           //0：开机或入住，1:关机或退房
};

struct service
{
    int roomId;         //房间号
    int customerId;     //客户号
    QString startTime;  //服务起始时间
    QString endTime;    //服务结束时间
    int windSpeed;      //风速
    int mode;           //模式
    double energy;      //能耗
    double fee;         //计费
};

class database
{
private:
    database();
    ~database();
    static database* instance;
    QSqlDatabase _db;

public:
    static database* getInstance();
    void insertCheck(struct operation entry);   //插入入住退房信息表
    void insertPower(struct operation entry);   //插入开机关机信息表
    void insertService(struct service entry);   //插入服务记录表
    QString getBill(int customerId);            //获得账单
    QString getDetailBill(int customerId);      //获得详单
};

#endif // DATABASE_H
