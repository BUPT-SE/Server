#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <QJsonObject>
#include <QString>

class Attribute
{
public:
    static int SPD_LOW = 0;             //风速低档
    static int SPD_MID = 1;             //风速中档
    static int SPD_HIGH = 2;            //风速高档
    static int MODE_COOL = 0;           //工作模式制冷
    static int MODE_HEAT = 1;           //工作模式制热

public:
    Attribute();                        //构造函数
    ~Attribute();                       //析构函数
    QJsonObject toJson();               //把属性转化成Json格式
    void setFromJson();                 //从Json格式改变属性值
    void incRoomTmp();                  //室温升一度
    void decRoomTmp();                  //室温降一度

    //各属性的get和set方法
    QString getRoomNum() const;
    void setRoomNum(const QString &roomNum);
    int getWindSpeed() const;
    void setWindSpeed(const int &windSpeed);
    float getRoomTmp() const;
    int getMode() const;
    float getTargetTmp() const;
    void setTargetTmp(float targetTmp);
    bool getPower() const;
    void setPower(bool power);
    bool getIsServed() const;
    float getKWh() const;
    flaot getFee() const;

private:
    QString _roomNum;                   //房间号
    int _windSpeed;                     //风速，低档，中档，高档
    float _roomTmp;                     //室温，构造函数中=_defRoomTmp
    int _mode;                          //工作模式，制冷，制热，启动之后从主机获取
    float _targetTmp;                   //目标温度，构造函数中赋初值，启动之后从主机获取
    bool _power;                        //开关机
    bool _isServed;                     //是否正在被服务
    float _defRoomTmp;                  //缺省室温，室温稳定值，构造函数中赋初值
    float _kWh;                         //消耗的电量
    flaot _fee;                         //所需支付的费用
};

#endif // ATTRIBUTE_H
