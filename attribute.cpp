#include "attribute.h"
#include "server.h"
#include "ui_server.h"
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

void Attribute::setWindSpeed(const QString &windSpeed)
{
    if(windSpeed.operator ==("Low"))
        _windSpeed=0;
    else if(windSpeed.operator ==("Middle"))
        _windSpeed=1;
    else if(windSpeed.operator ==("High"))
        _windSpeed=2;
}

float Attribute::getRoomTmp() const
{
    return _roomTmp;
}

int Attribute::getMode() const////////////////////////////////////////
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

float Attribute::getFee() const
{
    return _fee;
}
