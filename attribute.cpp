#include "attribute.h"

Attribute::Attribute()
{
    this->_power=false;
}

Attribute::~Attribute()
{

}

void Attribute::setFromJson(QByteArray byteArray)
{
    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(byteArray, &jsonError);
    if(jsonError.error == QJsonParseError::NoError)
    {
        if(doucment.isObject())
        {
            QJsonObject json = doucment.object();
            if(json.contains("roomNum"))
            {
                QJsonValue roomNum=json.take("roomNum");
                this->_roomNum=roomNum.toInt();
                qDebug() << _roomNum;
            }
            if(json.contains("windSpeed"))
            {
                QJsonValue windSpeed=json.take("windSpeed");
                this->_windSpeed=windSpeed.toInt();
                 qDebug() << _windSpeed;
            }
            if(json.contains("roomTmp"))
            {
                QJsonValue roomTmp=json.take("roomTmp");
                this->_roomTmp=roomTmp.toDouble();
                 qDebug() << _roomTmp;
            }
            if(json.contains("targetTmp"))
            {
                QJsonValue targetTmp=json.take("targetTmp");
                this->_targetTmp=targetTmp.toDouble();
                 qDebug() << _targetTmp;
            }
            if(json.contains("power"))
            {
                QJsonValue power=json.take("power");
                this->_power=power.toBool();
                 qDebug() << _power;
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
    json.insert("isServed",_isServed);
    json.insert("Kwh",_Kwh);
    json.insert("fee",_fee);
    return json;
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

void Attribute::setWindSpeed(const int &windSpeed)
{
    _windSpeed = windSpeed;
}

double Attribute::getRoomTmp() const
{
    return _roomTmp;
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

void Attribute::setTargetTmp(double targetTmp)
{
    _targetTmp = targetTmp;
}

bool Attribute::getPower() const
{
    return _power;
}

void Attribute::setPower(bool power)
{
    _power = power;
}

bool Attribute::getIsServed() const
{
    return _isServed;
}

double Attribute::getKWh() const
{
    return _Kwh;
}

double Attribute::getFee() const
{
    return _fee;
}
