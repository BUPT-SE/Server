#include "attribute.h"
#include <QDebug>

Attribute::Attribute()
{
    //初始化
    _Kwh = 0;
    _fee = 0;
    _power = false;
    _isServed = false;
}

Attribute::~Attribute()
{

}

void Attribute::setFromJson(QByteArray byteArray)
{
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(byteArray, &jsonError);
    qDebug() << "recieve message from client.";
    qDebug() << byteArray;
    if(jsonError.error == QJsonParseError::NoError)
    {
        if(doucment.isObject())
        {
            QJsonObject json = doucment.object();
            if(json.contains("roomNum"))
            {
                QJsonValue roomNum=json.take("roomNum");
                this->_roomNum=roomNum.toInt();
                //qDebug() << _roomNum;
            }
            if(json.contains("windSpeed"))
            {
                QJsonValue windSpeed=json.take("windSpeed");
                this->_windSpeed=windSpeed.toInt();
                //qDebug() << _windSpeed;
            }
            if(json.contains("roomTmp"))
            {
                QJsonValue roomTmp=json.take("roomTmp");
                this->_roomTmp=roomTmp.toDouble();
                //qDebug() << _roomTmp;
            }
            if(json.contains("targetTmp"))
            {
                QJsonValue targetTmp=json.take("targetTmp");
                this->_targetTmp=targetTmp.toDouble();
                //qDebug() << _targetTmp;
            }
            if(json.contains("power"))
            {
                QJsonValue power=json.take("power");
                this->_power=power.toBool();
                //qDebug() << _power;
            }
            else
               qDebug() << jsonError.error;
        }
    }
}

QJsonObject Attribute::toJson()
{
    QJsonObject json;
    json.insert("roomTmp",_roomTmp);
    json.insert("mode",_mode);
    json.insert("isServed", _isServed);
    json.insert("Kwh",_Kwh);
    json.insert("fee",_fee);
    return json;
}

void Attribute::incRoomTmp()
{
    _roomTmp += 0.1;
}

void Attribute::decRoomTmp()
{
    _roomTmp -= 0.1;
}

int Attribute::getRoomNum() const
{
    return _roomNum;
}

void Attribute::setRoomNum(const int &roomNum)
{
    _roomNum = roomNum;
}

int Attribute::getWindSpeed() const
{
    return _windSpeed;
}

double Attribute::getRoomTmp() const
{
    return _roomTmp;
}

void Attribute::setRoomTmp(const double roomTmp)
{
    _roomTmp = roomTmp;
}

void Attribute::setMode(const int mode)
{
    this->_mode=mode;
}

int Attribute::getMode() const
{
    return _mode;
}

double Attribute::getTargetTmp() const
{
    return _targetTmp;
}

void Attribute::setTargetTmp(const double targetTmp)
{
    _targetTmp = targetTmp;
}

bool Attribute::getPower() const
{
    return _power;
}

void Attribute::setPower(const bool power)
{
    _power = power;
}

void Attribute::setIsServed(const bool isServed)
{
    _isServed = isServed;
    qDebug() << _isServed;
}

bool Attribute::getIsServed() const
{
    return _isServed;
}

void Attribute::setKwh(const double Kwh)
{
    _Kwh = Kwh;
}

double Attribute::getKwh() const
{
    return _Kwh;
}

void Attribute::setLowestTmp(const double lowestTmp)
{
    _lowestTmp = lowestTmp;
}

void Attribute::setHighestTmp(const double highestTmp)
{
    _highestTmp = highestTmp;
}

double Attribute::getLowestTmp() const 
{
	return _lowestTmp;
}

double Attribute::getHighestTmp() const
{
	return _highestTmp;
}

void Attribute::setFee(const double fee)
{
    _fee = fee;
}

double Attribute::getFee() const
{
    return _fee;
}
