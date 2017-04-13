#include "clientblock.h"

ClientBlock::ClientBlock(QTcpSocket* socket,int mode, QObject *parent)
{
    this->_socket=socket;
    this->setMode(mode);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readMessage()));
}



void ClientBlock::setMode(int mode)
{
    this->_attribute.setMode(mode);
}

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
