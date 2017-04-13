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

void Attribute::setWindSpeed(const QString &windSpeed)
{
    if(windSpeed.operator ==("Low"))
        _windSpeed=0;
    else if(windSpeed.operator ==("Middle"))
        _windSpeed=1;
    else if(windSpeed.operator ==("High"))
        _windSpeed=2;
}


int Attribute::getMode() const
{
    return _mode;
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

void Attribute::setFee(double fee)
{
    _fee = fee;
}

void Attribute::setKWh(double kWh)
{
    _kWh = kWh;
}

double Attribute::getRoomTmp() const
{
    return _roomTmp;
}

void Attribute::setRoomTmp(double roomTmp)
{
    _roomTmp = roomTmp;
}

double Attribute::getTargetTmp() const
{
    return _targetTmp;
}

void Attribute::setTargetTmp(double targetTmp)
{
    _targetTmp = targetTmp;
}

double Attribute::getFee() const
{
    return _fee;
}
