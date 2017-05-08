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
    _power = false;
    _mode = mode;
    _suspended = 0;
    _count = 0;
    _tmpFee = 0;
    _tmpKwh = 0;
    _fee = 0;
    _Kwh = 0;
    _suspended = 0;
    _isSatisfied = false;

    //连接信号和槽
    connect(socket, SIGNAL(readyRead()), this, SLOT(readMessage()));
}

ClientBlock::~ClientBlock()
{

}

int ClientBlock::getPriority()
{
    int p;//优先级越大，数字越小
    //优先级与风速有关，风速越大，优先级越大
    p = 3 - _windSpeed;

    return p;
}

void ClientBlock::setID(int id)
{
    _id = id;
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

    _socket->write((byteArray));
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

void ClientBlock::setSysTime(QString time)
{
    _sysTime = time;
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
            QJsonValue fee = json.take("fee");
            QJsonValue Kwh = json.take("Kwh");

            //如果是第一次请求
            if(power.toBool() == true && _power == false)
            {
                //写入副本
                _roomNum = roomNum.toInt();
                _roomTmp = roomTmp.toDouble();
                _windSpeed = windSpeed.toInt();
                _power = power.toBool();
                _fee = fee.toDouble();
                _Kwh = Kwh.toDouble();

                emit isCheckedIn(this);
                qDebug() << "check in???" << _isCheckedIn;
                if(!_isCheckedIn)
                {
                    _socket->disconnectFromHost();
                    emit shutdown(this);
                    return ;
                }

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
                //数据库写入
                operation oper;
                oper.roomId = _roomNum;
                oper.customerId = _id;
                oper.oper = 1;//关机
                database::getInstance()->insertPower(oper);
                //销毁自身
                emit shutdown(this);
                return ;
            }
            //写入副本
            _roomNum = roomNum.toInt();
            //室温变化超过一度则认为是变温请求
            if(_isSatisfied && qAbs(roomTmp.toDouble() - _roomTmp) >= 1)
            {
                _roomTmp = roomTmp.toDouble();
                _isSatisfied = false;
            }
            _targetTmp = targetTmp.toDouble();
            _power = power.toBool();
            if(windSpeed.toInt() != _windSpeed)
            {
                _windSpeed = windSpeed.toInt();
                updateCount();
            }
            //更新UI
            emit update(this);
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
    if(!_isSatisfied)
    {
        if(_isServed)//服务中
        {
            qDebug() << "count:" << _count;
            if(_count != 0) //未到变化温度的时机
                _count--;
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
                qDebug() << _mode << _targetTmp << _roomTmp;
                if(_mode == MODE_COOL && _targetTmp > _roomTmp)
                {

                    _isSatisfied = true;
                    _isServed = false;
                    sendMessage();//发送消息,停止服务
                    //数据库存储
                    _tmpFee = 0;
                    _tmpKwh = 0;

                }
                //若当前为制热模式且室温高于目标温度，则停止服务
                else if(_mode == MODE_HEAT && _targetTmp < _roomTmp)
                {
                    _isSatisfied = true;
                    _isServed = false;
                    sendMessage();//发送消息,停止服务

                    //数据库存储
                    _tmpFee = 0;
                    _tmpKwh = 0;
                }
                else
                {
                    updateCount();
                    sendMessage();//发送消息,更新温度和费用
                }
            }
            //更新UI
            emit update(this);
        }
        else//挂起
        {
            _suspended++;//记录挂起时间
        }
    }
}

void ClientBlock::updateCount()
{
    switch(_windSpeed)
    {
    case SPD_LOW:   //低风速每3分钟变化1温度
        _count = 3 * 60 / 10.0;
        break;
    case SPD_MID:   //中风速每2分钟变化1温度
        _count = 2 * 60 / 10;
        break;
    case SPD_HIGH:  //高风速每2分钟变化1温度
        _count = 60 / 10;
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

QJsonObject ClientBlock::toJson()
{
    QJsonObject json;
    json.insert("roomNum",_roomNum);
    json.insert("roomTmp",_roomTmp);
    json.insert("mode",_mode);
    json.insert("isServed", _isServed);
    json.insert("Kwh",_Kwh);
    json.insert("fee",_fee);
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

