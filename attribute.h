#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include <QJsonObject>
#include <QString>
#include <QJsonDocument>
#include <QtNetwork>

class Attribute
{
public:
    static const int SPD_LOW = 0;               //风速低档
    static const int SPD_MID = 1;               //风速中档
    static const int SPD_HIGH = 2;              //风速高档
    static const int MODE_COOL = 0;             //工作模式制冷
    static const int MODE_HEAT = 1;             //工作模式制热

public:
    Attribute();                                //构造函数
    ~Attribute();                               //析构函数
    QJsonObject toJson();                       //把属性转化成Json格式
    void setFromJson(QByteArray byteArray);     //从Json格式改变属性值
    void incRoomTmp();                          //室温升0.1度
    void decRoomTmp();                          //室温降0.1度

    //各属性的get和set方法
    int getRoomNum() const;
    void setRoomNum(const int &roomNum);
    int getWindSpeed() const;
    double getRoomTmp() const;
    void setRoomTmp(const double roomTmp);
    int getMode() const;
    void setMode(const int mode);
    double getTargetTmp() const;
    void setTargetTmp(const double targetTmp);
    bool getPower() const;
    void setPower(const bool power);
    void setIsServed(const bool isServed);
    bool getIsServed() const;
    void setLowestTmp(const double lowestTmp);
	double getLowestTmp() const;
    void setHighestTmp(const double highestTmp);
	double getHighestTmp() const;
    void setFee(const double fee);
    double getFee() const;
    void setKwh(const double Kwh);
    double getKwh() const;

private:
    int _roomNum;                       //房间号
    int _windSpeed;                     //风速，低档，中档，高档
    double _roomTmp;                    //室温，构造函数中=_defRoomTmp
    int _mode;                          //工作模式，制冷，制热，启动之后从主机获取
    double _targetTmp;                  //目标温度，构造函数中赋初值，启动之后从主机获取
    bool _power;                        //开关机
    bool _isServed;                     //是否正在被服务
    double _Kwh;                        //消耗的电量
    double _fee;                        //所需支付总的费用
    double _lowestTmp;                  //允许设置目标温度的最低温度
    double _highestTmp;                 //运行设置目标温度的最高温度
};

#endif // ATTRIBUTE_H
