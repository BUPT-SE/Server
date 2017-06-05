#include "clientblock.h"
#include <QDebug>

ClientBlock::ClientBlock(QTcpSocket* socket, double lowestTmp, double highestTmp,
                         double targetTmp, int mode, QObject *parent) : QObject(parent)
{
    _id = -1;
    _isCheckedIn = true;
    _socket = socket;
    _lowestTmp = lowestTmp;
    _highestTmp = highestTmp;
    _targetTmp = targetTmp;
    _isServed = false;
    _isSatisfied = false;
    _tmpSatisfied = false;
    _power = false;
    _mode = mode;
    _count = 0;
    _tmpFee = 0;
    _tmpKwh = 0;
    _fee = 0;
    _Kwh = 0;
    _servedTime = 0;
    _isSatisfied = false;
    _tmpWindSpeed = -1;

    //连接信号和槽
    connect(socket, SIGNAL(readyRead()), this, SLOT(readMessage()));
}

ClientBlock::~ClientBlock()
{

}

void ClientBlock::setID(int id)
{
    _id = id;
}

int ClientBlock::getID()
{
    return _id;
}

void ClientBlock::setIsCheckedIn(bool flag)
{
    _isCheckedIn = flag;
}

int ClientBlock::getRoomNum()
{
    return _roomNum;
}

double ClientBlock::getRoomTmp()
{
    return _roomTmp;
}

double ClientBlock::getTargetTmp()
{
    return _targetTmp;
}

int ClientBlock::getWindSpeed()
{
    return _windSpeed;
}

int ClientBlock::getMode()
{
    return _mode;
}

double ClientBlock::getFee()
{
    return _fee;
}

double ClientBlock::getKwh()
{
    return _Kwh;
}

void ClientBlock::sendMessage()
{
    QJsonDocument document;
    document.setObject(toJson());
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);

    qDebug() << "send message to client!";
    qDebug() << byteArray;

    _socket->write(byteArray);
    _socket->flush();
}

void ClientBlock::sendFirstMessage()
{
    qDebug() << "send first message to client!";
    QJsonDocument document;
    QJsonObject json;

    json.insert("mode", _mode);
    json.insert("lowestTmp", _lowestTmp);
    json.insert("highestTmp", _highestTmp);
    json.insert("targetTmp", _targetTmp);
    json.insert("roomTmp", _roomTmp);
    json.insert("isServed", _isServed);
    document.setObject(json);

    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    _socket->write(byteArray);
}

void ClientBlock::setFee(double fee)
{
    _fee = fee;
}

void ClientBlock::setKwh(double Kwh)
{
    _Kwh = Kwh;
}

void ClientBlock::readMessage()
{
    QByteArray byteArray = _socket->readAll();
    qDebug() << "recieve from client !";
    qDebug() << byteArray;

    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(byteArray, &jsonError);
    QJsonObject json = doucment.object();

    if(jsonError.error == QJsonParseError::NoError)
    {
        if(doucment.isObject())
        {
            QJsonValue roomNum = json.take("roomNum");
            QJsonValue roomTmp = json.take("roomTmp");
            QJsonValue targetTmp = json.take("targetTmp");
            QJsonValue windSpeed = json.take("windSpeed");
            QJsonValue power = json.take("power");

            //如果从机开机
            if(power.toBool() == true && _power == false)
            {
                //写入副本
                _roomNum = roomNum.toInt();
                _roomTmp = roomTmp.toDouble();
                _windSpeed = windSpeed.toInt();
                _tmpWindSpeed = _windSpeed;
                _power = power.toBool();

                emit isCheckedIn(this);
                qDebug() << "check in???" << _isCheckedIn;
                if(!_isCheckedIn)
                {
                    _socket->disconnectFromHost();
                    emit shutdown(this);
                    return ;
                }
                //同步账单信息
                emit updateBill(this);
                //记录服务起始时间
                _startTime = getCurTime();
                //发送第一条消息
                sendFirstMessage();
                updateCount();
                //数据库写入
                operation oper;
                oper.roomId = _roomNum;
                oper.customerId = _id;
                oper.oper = 0;//开机
                database::getInstance()->insertPower(oper);
                //更新UI
                emit update(this);
                return;
            }
            //如果从机关机
            else if(power.toBool() == false && _power == true)
            {
                //断开连接
                qDebug() << "Client shut down!";
                _socket->disconnectFromHost();
                //销毁自身
                emit shutdown(this);
                return;
            }
            //被挂起时从机向主机同步室温
            if(_isServed == false && _isSatisfied == false)
            {
               //记录服务起始时间
               _startTime = getCurTime();
               if(qAbs(roomTmp.toDouble() - _roomTmp) >= 1)
               {
                   _roomTmp = roomTmp.toDouble();
                   _tmpSatisfied = false;
               }
               _targetTmp = targetTmp.toDouble();
               if(windSpeed.toInt() != _windSpeed)
               {
                   _tmpWindSpeed = _windSpeed;
                   _windSpeed = windSpeed.toInt();
                   _tmpSatisfied = false;
                   updateCount();
                   emit dispatch();
               }
            }
            //满足服务后从机向主机同步室温
            else if(_isServed == false && _isSatisfied == true)
            {
               if(qAbs(roomTmp.toDouble() - _roomTmp) >= 1)
               {
                   //记录服务起始时间
                   _startTime = getCurTime();
                   _roomTmp = roomTmp.toDouble();
                   _isSatisfied = false;
                   _tmpSatisfied = false;
               }
               _targetTmp = targetTmp.toDouble();
               if(windSpeed.toInt() != _windSpeed)
               {
                   //记录服务起始时间
                   _startTime = getCurTime();
                   _windSpeed = windSpeed.toInt();
                   _isSatisfied = false;
                   _tmpSatisfied = false;
                   updateCount();
                   emit dispatch();
               }
            }
            else if(_isServed)
            {
                qDebug() << "!!!!!!@@@@";
                //记录服务起始时间
                _startTime = getCurTime();
                _targetTmp = targetTmp.toDouble();
                if(windSpeed.toInt() != _windSpeed)
                {
                    _tmpWindSpeed = _windSpeed;
                    _windSpeed = windSpeed.toInt();
                    updateCount();
                }
            }
            //更新UI
            emit update(this);
            //写入数据库
            service ser;
            ser.customerId = _id;
            ser.roomId = _roomNum;
            ser.mode = _mode;
            ser.energy = _tmpKwh;
            ser.fee = _tmpFee;
            ser.windSpeed = _windSpeed;
            ser.startTime = _startTime;
            database::getInstance()->insertService(ser);

            _tmpFee = 0;
            _tmpKwh = 0;
        }
    }
}

/*先判断是否服务完成，再判断是否被挂起（挂起不变），
 * 若不为0 count--, 若count==0，变温度，
 * 再判断是否达到目标温度，改变isFinished,
 * 发消息，重制count，
 */
void ClientBlock::check()
{
    if(!_tmpSatisfied)
    {
        if(_isServed)//服务中
        {
            qDebug() << "count:" << _count;
            if(_count != 0) //未到变化温度的时机
            {
                _servedTime++;
                _count--;
            }
            else            //时机已到
            {
                if(_mode == MODE_COOL)
                    decDeltaRoomTmp();            //降温
                else
                    incDeltaRoomTmp();            //升温

                _tmpFee += 0.1;
                _tmpKwh += 0.1;
                _fee += 0.1;
                _Kwh += 0.1;

                //若当前为制冷模式且目标温度高于室温，则停止服务
                if(_mode == MODE_COOL && _targetTmp > _roomTmp)
                {
                    _isSatisfied = true;
                    _tmpSatisfied = true;
                    _isServed = false;
                    sendMessage();//发送消息,停止服务
                    //数据库存储
                    service ser;
                    ser.customerId = _id;
                    ser.roomId = _roomNum;
                    ser.mode = _mode;
                    ser.energy = _tmpKwh;
                    ser.fee = _tmpFee;
                    ser.windSpeed = _windSpeed;
                    ser.startTime = _startTime;
                    database::getInstance()->insertService(ser);
                    _tmpFee = 0;
                    _tmpKwh = 0;
                }
                //若当前为制热模式且室温高于目标温度，则停止服务
                else if(_mode == MODE_HEAT && _targetTmp < _roomTmp)
                {
                    _isSatisfied = true;
                    _tmpSatisfied = true;
                    _isServed = false;
                    sendMessage();//发送消息,停止服务
                    //数据库存储
                    service ser;
                    ser.customerId = _id;
                    ser.roomId = _roomNum;
                    ser.mode = _mode;
                    ser.energy = _tmpKwh;
                    ser.fee = _tmpFee;
                    ser.windSpeed = _windSpeed;
                    ser.startTime = _startTime;
                    database::getInstance()->insertService(ser);
                    _tmpFee = 0;
                    _tmpKwh = 0;
                }
                else
                {
                    updateCount();
                    sendMessage();//发送消息,更新温度和费用
                }
                //更新UI
                emit update(this);
            }
        }
        else//挂起
        {
            _servedTime = 0;//记录挂起时间
        }
    }
}

void ClientBlock::updateCount()
{
    qDebug() << "tmpWindSpeed" << _tmpWindSpeed;
    if(_tmpWindSpeed != -1)
    {
        int c;
        switch(_tmpWindSpeed)
        {
        case SPD_LOW:   //低风速每3分钟变化1温度
            c = 18;
            break;
        case SPD_MID:   //中风速每2分钟变化1温度
            c = 12;
            break;
        case SPD_HIGH:  //高风速每2分钟变化1温度
            c = 6;
            break;
        }
        switch(_windSpeed)
        {
        case SPD_LOW:   //低风速每3分钟变化1温度
            _count = 18 * _count / c;
            break;
        case SPD_MID:   //中风速每2分钟变化1温度
            _count = 12 * _count / c;
            break;
        case SPD_HIGH:  //高风速每2分钟变化1温度
            _count = 6 * _count / c;
            break;
        }
        _tmpWindSpeed = -1;
    }
    else
        switch(_windSpeed)
        {
        case SPD_LOW:   //低风速每3分钟变化1温度
            _count = 18;
            break;
        case SPD_MID:   //中风速每2分钟变化1温度
            _count = 12;
            break;
        case SPD_HIGH:  //高风速每2分钟变化1温度
            _count = 6;
            break;
        default:
            _count = 100;
            break;
        }
}

bool ClientBlock::isSatisfied()
{
    return _isSatisfied;
}

bool ClientBlock::isServed()
{
    return _isServed;
}

void ClientBlock::setIsServed(bool isServed)
{
    _isServed = isServed;
}

void ClientBlock::setIsSatisfied(bool isSatisfied)
{
    _isSatisfied = isSatisfied;
}

QJsonObject ClientBlock::toJson()
{
    QJsonObject json;
    json.insert("roomNum",_roomNum);
    json.insert("roomTmp",_roomTmp);
    json.insert("mode",_mode);
    json.insert("isServed", _isServed);
    json.insert("Kwh",_Kwh);
    json.insert("fee",_fee);
    json.insert("windSpeed", _windSpeed);
    return json;
}

void ClientBlock::incDeltaRoomTmp()
{
    _roomTmp += DELTA;
}

void ClientBlock::decDeltaRoomTmp()
{
    _roomTmp -= DELTA;
}

int ClientBlock::getServedTime()
{
    return _servedTime;
}

int ClientBlock::getTmpFee()
{
    return _tmpFee;
}

int ClientBlock::getTmpKwh()
{
    return _tmpKwh;
}

bool ClientBlock::isTmpSatisfied()
{
    return _tmpSatisfied;
}

void ClientBlock::setTmpSatisfied(bool tmpSatisfied)
{
    _tmpSatisfied = tmpSatisfied;
}

QString ClientBlock::getCurTime()
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
