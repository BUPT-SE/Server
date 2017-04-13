#include "attribute.h"
#include "clientblock.h"

void ClientBlock::sendMessage()
{
    qDebug()<<"send message to client."<<endl;
    QJsonDocument document;
    document.setObject(_attribute.toJson());
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    _socket->write((byteArray));
}

void ClientBlock::sendFirstMessage()
{
    qDebug()<<"send first message to client."<<endl;

    QJsonDocument document;
    QJsonObject json;

    json.insert("mode",1);
    json.insert("lowestTmp",15.0);
    json.insert("highestTmp",30.0);
    json.insert("targetTmp",26);
    json.insert("isServed",false);
    document.setObject(json);
    QByteArray byteArray = document.toJson(QJsonDocument::Compact);
    _socket->write(byteArray);
}

void ClientBlock::readMessage()
{

    QByteArray byteArray = _socket->readAll();

    qDebug()<<byteArray;

    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(byteArray, &jsonError);
    QJsonObject json = doucment.object();

    if(jsonError.error == QJsonParseError::NoError)
    {

        qDebug()<<byteArray;
        if(doucment.isObject())
        {
            if(json.contains("power"))
            {
                QJsonValue power=json.take("power");
                //如果是第一次请求
                if(power.toBool()==true&&_attribute.getPower()==false)
                {
                    this->sendFirstMessage();
                }
                //如果从机关机
                else if(power.toBool()==false&&_attribute.getPower()==true)
                {
                    qDebug()<<"dhahdhashdshadhsahdhsahdh";
                    _socket->disconnectFromHost();
                    emit shutdown(this);
                }
                _attribute.setFromJson(byteArray);
            }
        }
    }
    //更新UI
}


ClientBlock::ClientBlock(QTcpSocket *socket, double lowestTmp, double highestTmp,double targetTmp, int mode, Server *server, QObject *parent)
            :QObject(parent)
{
    _socket = socket;
    _attribute.setLowestTmp(lowestTmp);
    _attribute.setHighestTmp(highestTmp);
    _attribute.setTargetTmp(targetTmp);
    _server=server;
    _attribute.setMode(mode);
    _suspended = 0;
    _count = 0;
    _tmpFee = 0;
    _suspended = 0;
    _satisfied = false;
    //连接信号和槽
    connect(socket, SIGNAL(readyRead()), this, SLOT(readMessage()));
}


/*先判断是否服务完成，再判断是否被挂起（挂起不变），
 * 若不为0 count--, 若count==0，变温度，
 * 再判断是否达到目标温度，改变isFinished,
 * 发消息，重制count，
 */
void ClientBlock::check()
{
    if(!_satisfied)
    {
        if(_attribute.getIsServed())//服务中
        {
            if(_count != 0) //未到变化温度的时机
                _count--;
            else            //时机已到
            {
                if(_attribute.getMode() == Attribute::MODE_COOL)
                    _attribute.decRoomTmp();            //降温
                else
                    _attribute.incRoomTmp();            //升温

                _tmpFee+=0.1;
                _attribute.setFee(_attribute.getFee()+0.1);

                if(_attribute.getTargetTmp() == _attribute.getRoomTmp())
                {
                    _attribute.setIsServed(false);
                    _satisfied = true;


                    //数据库存储

                    emit update(_attribute.getRoomNum());//更新客户端状态
                }else{
                    updateCount();
                }

                //sendMessage();//发送消息,更新温度和费用

            }
        }else{//挂起
            _suspended++;//记录挂起时间
        }
    }
}

void ClientBlock::updateCount()
{
    switch(_attribute.getWindSpeed())
    {
    case Attribute::SPD_LOW:
        _count = 3*60/10.0;
        break;
    case Attribute::SPD_MID:
        _count = 2*60/10;
        break;
    case Attribute::SPD_HIGH:
        _count = 60/10;
        break;
    }
}

Attribute ClientBlock::getAttribute(){
    return _attribute;
}

bool ClientBlock::isSatisfied(){
    return _satisfied;
}

