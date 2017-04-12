#include "attribute.h"

void Attribute::incRoomTmp()
{
    _roomTmp+=0.1;
}

void Attribute::decRoomTmp()
{
    _roomTmp-=0.1;
}

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

float Attribute::getFee() const
{
    return _fee;
}

void Attribute::setIsServed(bool isServed)
{
    _isServed = isServed;
}

void Attribute::setLowestTmp(double lowestTmp)
{
    _lowestTmp = lowestTmp;
}

void Attribute::setHighestTmp(double highestTmp)
{
    _highestTmp = highestTmp;
}

void Attribute::setMode(int mode)
{
    _mode = mode;
}
