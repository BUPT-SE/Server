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

                sendMessage();//发送消息,更新温度和费用

            }
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
