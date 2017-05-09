#include "database.h"
#include "server.h"
#include <QDebug>

database::database()
{
    //连接数据库
    ConfigDialog cd;
    if (cd.exec() == QDialog::Accepted)
    {
        qDebug() << "Accepted!";
        _db = QSqlDatabase::addDatabase("QMYSQL");
        _db.setDatabaseName("hotel");
        _db.setUserName(cd.getUserName());
        _db.setPassword(cd.getPassword());
        _db.setHostName(cd.getHostName());

        // 检测可连接性
        if (!_db.open())
        {
            qDebug() << "The database cannot be connected.";
        }
        else
        {
            qDebug() << "The database can be connected.";
        }
    }
    else qDebug() << "Error!";
}

database::~database()
{
    _db.close();
    if (instance == NULL) return;
    delete instance;
    instance = NULL;
}

database* database::getInstance()
{
    if (instance == NULL)
    {
        instance = new database;
    }
    return instance;
}

void database::insertCheck(struct operation entry)
{
    QSqlQuery query;
    query.prepare("INSERT INTO check_in_out "
                  "(room_id, customer_id, time, operation) "
                  "VALUE (:room_id, :customer_id, :time, :operation)");
    query.bindValue(":room_id", entry.roomId);
    query.bindValue(":customer_id", entry.customerId);
    query.bindValue(":time", QDateTime::fromString(getCurTime(), "yyyy-MM-dd hh:mm:ss"));
    query.bindValue(":operation", entry.oper);
    query.exec();
}

void database::insertPower(struct operation entry)
{
    QSqlQuery query;
    query.prepare("INSERT INTO power_on_off "
                  "(room_id, customer_id, time, operation) "
                  "VALUE (:room_id, :customer_id, :time, :operation)");
    query.bindValue(":room_id", entry.roomId);
    query.bindValue(":customer_id", entry.customerId);
    query.bindValue(":time", QDateTime::fromString(getCurTime(), "yyyy-MM-dd hh:mm:ss"));
    query.bindValue(":operation", entry.oper);
    query.exec();
}

void database::insertService(struct service entry)
{
    QSqlQuery query;
    query.prepare("INSERT INTO service_record "
                  "(room_id, customer_id, start_time, end_time, wind_speed, energy, mode, fee) "
                  "VALUE (:room_id, :customer_id, :start_time, :end_time, :wind_speed, :energy, :mode, :fee)");
    query.bindValue(":room_id", entry.roomId);
    query.bindValue(":customer_id", entry.customerId);
    query.bindValue(":start_time", QDateTime::fromString(getCurTime(), "yyyy-MM-dd hh:mm:ss"));
    query.bindValue(":end_time", QDateTime::fromString(getCurTime(), "yyyy-MM-dd hh:mm:ss"));
    query.bindValue(":wind_speed", entry.windSpeed);
    query.bindValue(":energy", entry.energy);
    query.bindValue(":mode", entry.mode);
    query.bindValue(":fee", entry.fee);
    query.exec();
}

QString database::getBill(int customerId)
{
    //查询入住及退房时间
    QSqlQuery query;
    query.prepare("SELECT time FROM check_in_out "
                  "WHERE customer_id = :customer_id");
    query.bindValue(":customer_id", customerId);
    query.exec();

    query.first();
    QDateTime checkinTime = query.value(0).toDateTime();
    query.last();
    QDateTime checkoutTime = query.value(0).toDateTime();

    //计算总消费
    query.prepare("SELECT sum(fee) as tot_fee FROM service_record "
                  "WHERE customer_id = :customer_id");
    query.bindValue(":customer_id", customerId);
    query.exec();

    query.first();
    QString result = QString::fromLocal8Bit("客户:") + QString::number(customerId) + "\n" +
            QString::fromLocal8Bit("入住时间:") + checkinTime.toString("yyyy-MM-dd hh:mm:ss") + "\n" +
            QString::fromLocal8Bit("退房时间:") + checkoutTime.toString("yyyy-MM-dd hh:mm:ss") + "\n" +
            QString::fromLocal8Bit("总消费:") + QString::number(query.value(0).toDouble()) + QString::fromLocal8Bit("元");

    return result;
}

QString database::getDetailBill(int customerId)
{
    //查询入住及退房时间
    QSqlQuery querycheck;
    querycheck.prepare("SELECT time FROM check_in_out "
                  "WHERE customer_id = :customer_id");
    querycheck.bindValue(":customer_id", customerId);
    querycheck.exec();

    querycheck.first();
    QDateTime checkinTime = querycheck.value(0).toDateTime();
    querycheck.last();
    QDateTime checkoutTime = querycheck.value(0).toDateTime();

    QString result = QString::fromLocal8Bit("客户:") + QString::number(customerId) + "\n" +
            QString::fromLocal8Bit("入住时间:") + checkinTime.toString("yyyy-MM-dd hh:mm:ss") + "\n" +
            QString::fromLocal8Bit("退房时间:") + checkoutTime.toString("yyyy-MM-dd hh:mm:ss") + "\n" +
            QString::fromLocal8Bit("详细计费信息如下:") + "\n";

    //查询开机及关机时间
    QSqlQuery querypower;
    querypower.prepare("SELECT time FROM power_on_off "
                       "WHERE customer_id = :customer_id");
    querypower.bindValue(":customer_id", customerId);
    querypower.exec();

    //查询详细服务记录
    QSqlQuery subquery;
    while (querypower.next()) {
        QDateTime poweronTime = querypower.value(0).toDateTime();
        querypower.next();
        QDateTime poweroffTime = querypower.value(0).toDateTime();

        result += QString::fromLocal8Bit("在 ") + poweronTime.toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit(" 开机") + "\n";

        subquery.prepare("SELECT * FROM service_record "
                         "WHERE customer_id = :customer_id AND "
                         "start_time >= :poweronTime AND "
                         "end_time <= :poweroffTime");
        subquery.bindValue(":customer_id", customerId);
        subquery.bindValue(":poweronTime", poweronTime);
        subquery.bindValue(":poweroffTime", poweroffTime);
        subquery.exec();

        while (subquery.next()) {
            //详单时间信息
            result += QString::fromLocal8Bit("服务房间：%1 \n"
                                             "服务起始时间：%2 \n"
                                             "服务结束时间：%3 \n")
                    .arg(subquery.value(0).toInt())
                    .arg(subquery.value(2).toDateTime().toString("yyyy-MM-dd hh:mm:ss"))
                    .arg(subquery.value(3).toDateTime().toString("yyyy-MM-dd hh:mm:ss"));

            //详单风速信息
            if (subquery.value(4).toInt() == 0)
            {
                result += QString::fromLocal8Bit("风速： 低 \n");
            }
            else if (subquery.value(4).toInt() == 1)
            {
                result += QString::fromLocal8Bit("风速： 中 \n");
            }
            else
            {
                result += QString::fromLocal8Bit("风速： 高 \n");
            }

            //详单模式信息
            if (subquery.value(5).toInt() == 0)
            {
                result += QString::fromLocal8Bit("工作模式：制冷 \n");
            }
            else
            {
                result += QString::fromLocal8Bit("工作模式：制热 \n");
            }

            //详单能量费用信息
            result += QString::fromLocal8Bit("消耗能量：%1 KWH \n"
                              "该次消费金额：%2 元 \n")
                    .arg(subquery.value(6).toDouble())
                    .arg(subquery.value(7).toDouble());
        }

        result += QString::fromLocal8Bit("在 ") + poweroffTime.toString("yyyy-MM-dd hh:mm:ss") + QString::fromLocal8Bit(" 关机 \n");
    }

    //计算总消费
    QSqlQuery query;
    query.prepare("SELECT sum(fee) as tot_fee FROM service_record "
                  "WHERE customer_id = :customer_id");
    query.bindValue(":customer_id", customerId);
    query.exec();
    query.first();
    result += QString::fromLocal8Bit("总消费:") + QString::number(query.value(0).toDouble()) + QString::fromLocal8Bit("元");

    return result;
}

QString database::getCurTime()
{
    QDateTime Date = QDateTime::currentDateTime();
    QTime time = QTime::currentTime();

    int year=Date.date().year();
    int month=Date.date().month();
    int day=Date.date().day();
    int hour=time.hour();
    int min=time.minute();
    int sec = time.second();

    QString sysTime;
    sysTime = QString::number(year);
    sysTime += "-" + QString::number(month);
    sysTime += "-" + QString::number(day);
    sysTime += " " + QString::number(hour);
    sysTime += ":" + QString::number(min);
    sysTime += ":" + QString::number(sec);

    return sysTime;
}

