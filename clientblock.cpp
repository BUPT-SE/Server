#include "clientblock.h"


ClientBlock::ClientBlock(QTcpSocket *socket, double lowestTmp, double highestTmp,double targetTmp, int mode, Server *server, QObject *parent)
            :QObject(parent)
{
    _socket = socket;
    _attribute.setLowestTmp(lowestTmp);
    _attribute.setHighestTmp(highestTmp);
    _attribute.setTargetTmp(targetTmp);
    _server=server;
    _attribute.setMode(mode);
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
                    _attribute.decRoomTmp();
                else
                    _attribute.incRoomTmp();
                if(_attribute.getTargetTmp() == _attribute.getRoomTmp())
                {
                    _attribute.setIsServed(false);
                    sendMessage();//满足要求，发送消息
                    _satisfied = true;
                    emit update(_attribute.getRoomNum());//更新客户端状态
                }else{
                    updateCount();
                }
            }
        }
    }
}
