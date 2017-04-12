#include "attribute.h"

QString Attribute::getRoomNum() const
{
    return _roomNum;
}

void Attribute::setRoomNum(const QString &roomNum)
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

float Attribute::getRoomTmp() const
{
    return _roomTmp;
}

int Attribute::getMode() const
{
    return _mode;
}

float Attribute::getTargetTmp() const
{
    return _targetTmp;
}

void Attribute::setTargetTmp(float targetTmp)
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

float Attribute::getKWh() const
{
    return _kWh;
}

flaot Attribute::getFee() const
{
    return _fee;
}
