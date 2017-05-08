#include <QTimer>
#include <QDateTime>
#include <QVector>
#include <QtAlgorithms>
#include "server.h"
#include "ui_server.h"
#include "room.h"
#include "database.h"

Server::Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Server),
    _timer(new QTimer())
{
    _t = 0;
    _nextClientID = 1;
    ui->setupUi(this);

    //初始化tcpServer监听所有的连接请求
    _tcpServer = new QTcpServer();
    if(!_tcpServer->listen(QHostAddress::Any, 6666))
    {
        qDebug() <<"Contect Error";
    }
    else
    {
       qDebug() <<"Contect Succeed";
    }
    connect(_tcpServer, SIGNAL(newConnection()), this, SLOT(newConnection()));

    //四个房间和ui上控件指针关联
    room room1,room2,room3,room4;
    room1.id=ui->id1;
    room1.roomTmp=ui->roomTmp1;
    room1.roomNum=ui->roomNum1;
    room1.targetTmp=ui->targetTmp1;
    room1.windSpeed=ui->windSpeed1;
    room1.status=ui->status1;
    room1.Kwh=ui->Kwh1;
    room1.fee=ui->fee1;
    room1.check=ui->check1;

    room2.id=ui->id2;
    room2.roomTmp=ui->roomTmp2;
    room2.roomNum=ui->roomNum2;
    room2.targetTmp=ui->targetTmp2;
    room2.windSpeed=ui->windSpeed2;
    room2.status=ui->status2;
    room2.Kwh=ui->Kwh2;
    room2.fee=ui->fee2;
    room2.check=ui->check2;

    room3.id=ui->id3;
    room3.roomTmp=ui->roomTmp3;
    room3.roomNum=ui->roomNum3;
    room3.targetTmp=ui->targetTmp3;
    room3.windSpeed=ui->windSpeed3;
    room3.status=ui->status3;
    room3.Kwh=ui->Kwh3;
    room3.fee=ui->fee3;
    room3.check=ui->check3;

    room4.id=ui->id4;
    room4.roomTmp=ui->roomTmp4;
    room4.targetTmp=ui->targetTmp4;
    room4.windSpeed=ui->windSpeed4;
    room4.roomNum=ui->roomNum4;
    room4.status=ui->status4;
    room4.Kwh=ui->Kwh4;
    room4.fee=ui->fee4;
    room4.check=ui->check4;

    rooms.push_back(room1);
    rooms.push_back(room2);
    rooms.push_back(room3);
    rooms.push_back(room4);

    ui->paylist1->setEnabled(false);
    ui->detail1->setEnabled(false);
    ui->paylist2->setEnabled(false);
    ui->detail2->setEnabled(false);
    ui->paylist3->setEnabled(false);
    ui->detail3->setEnabled(false);
    ui->paylist4->setEnabled(false);
    ui->detail4->setEnabled(false);

    ui->onButton->setEnabled(false);
    ui->offButton->setEnabled(false);

    ui->check1->setEnabled(false);
    ui->check2->setEnabled(false);
    ui->check3->setEnabled(false);
    ui->check4->setEnabled(false);

    ui->Room1->setEnabled(false);
    ui->Room2->setEnabled(false);
    ui->Room3->setEnabled(false);
    ui->Room4->setEnabled(false);
    ui->configBox->setEnabled(true);

    //连接定时器和槽
    connect(_timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
}

Server::~Server()
{
    delete ui;
}

void Server::newConnection()
{
    //判断主机工作模式
    int mode;
    if(ui->coolRadio->isChecked())
        mode = 0;
    else if(ui->heatRadio->isChecked())
        mode = 1;

    //新建一个ClientBlock并压入队列
    ClientBlock* newClientBlock = new ClientBlock(_tcpServer->nextPendingConnection(),
                                                  ui->lowestTmpEdit->text().toDouble(),
                                                  ui->highestTmpEdit->text().toDouble(),
                                                  ui->targetTmpEdit->text().toDouble(),
                                                  mode, 0);
    this->_queue.push_back(newClientBlock);

    //连接信号和槽
    connect(newClientBlock, SIGNAL(shutdown(ClientBlock*)), this, SLOT(disConnection(ClientBlock*)));
    connect(newClientBlock, SIGNAL(isCheckedIn(ClientBlock*)), this, SLOT(checkIsCheckedIn(ClientBlock*)));
    connect(newClientBlock, SIGNAL(update(ClientBlock*)), this, SLOT(updateUI(ClientBlock*)));
}

void Server::disConnection(ClientBlock* client)
{
    QList<ClientBlock*>::iterator it;
    qDebug() << "queue length!!!!!!" << _queue.size() << endl;
    for(it = _queue.begin(); it != _queue.end(); it++)
        {
            //如果找到了这个房间对应的block
            if(*it == client)
            {
                //更新UI
                rooms[client->getRoomNum()].status->setText(QString::fromLocal8Bit("关机"));
                //从队列中删除
                _queue.erase(it);
                break;
            }
        }
    qDebug() << "queue length" << _queue.size()<<endl;
    delete client;
}

void Server::on_check1_clicked()
{
    if(ui->check1->text()=="Check in"){
        _clientID[0]=_nextClientID;
        _nextClientID++;
        ui->Room1->setEnabled(true);
        ui->paylist1->setEnabled(false);
        ui->detail1->setEnabled(false);

        rooms[0].id->setText(QString::number(_clientID[0],10));
        rooms[0].roomNum->setText("0");
        rooms[0].Kwh->setText("0");
        rooms[0].fee->setText("0");
        ui->check1->setText("Check out");

        //数据库写入
        operation oper;
        oper.customerId = _clientID[0];
        oper.roomId = 0;
        oper.oper = 0;//check in
        oper.time = _sysTime;
        database::getInstance()->insertCheck(oper);
    }
    else{
        ui->paylist1->setEnabled(true);
        ui->detail1->setEnabled(true);
        ui->check1->setText("Check in");
        rooms[0].status->setText(QString::fromLocal8Bit("关机"));
        ui->Room1->setEnabled(false);

        for(QList<ClientBlock*>::iterator it = _queue.begin(); it != _queue.end(); it++)
            if((*it)->getRoomNum() == 0)
            {
                (*it)->_socket->disconnectFromHost();
                delete (*it);
                _queue.erase(it);
                //数据库写入
                operation oper;
                oper.customerId = _clientID[0];
                oper.roomId = 0;
                oper.oper = 1;//关机
                oper.time = _sysTime;
                database::getInstance()->insertPower(oper);
                break;
            }

        //数据库写入
        operation oper;
        oper.customerId = _clientID[0];
        oper.roomId = 0;
        oper.oper = 1;//check out
        oper.time = _sysTime;
        database::getInstance()->insertCheck(oper);
    }
}

void Server::on_check2_clicked()
{
    if(ui->check2->text()=="Check in"){
        _clientID[1]=_nextClientID;
        _nextClientID++;
        ui->Room2->setEnabled(true);
        ui->paylist2->setEnabled(false);
        ui->detail2->setEnabled(false);
        rooms[1].id->setText(QString::number(_clientID[1],10));
        rooms[1].roomNum->setText("1");
        rooms[1].Kwh->setText("0");
        rooms[1].fee->setText("0");
        ui->check2->setText("Check out");

        //数据库写入
        operation oper;
        oper.customerId = _clientID[1];
        oper.roomId = 1;
        oper.oper = 0;//check in
        oper.time = _sysTime;
        database::getInstance()->insertCheck(oper);
    }
    else{
        ui->paylist2->setEnabled(true);
        ui->detail2->setEnabled(true);
        ui->check2->setText("Check in");
        rooms[1].status->setText(QString::fromLocal8Bit("关机"));

        for(QList<ClientBlock*>::iterator it = _queue.begin(); it != _queue.end(); it++)
            if((*it)->getRoomNum() == 1)
            {
                (*it)->_socket->disconnectFromHost();
                delete (*it);
                _queue.erase(it);
                //数据库写入
                operation oper;
                oper.customerId = _clientID[1];
                oper.roomId = 1;
                oper.oper = 1;//关机
                oper.time = _sysTime;
                database::getInstance()->insertPower(oper);
                break;
            }

        //数据库写入
        operation oper;
        oper.customerId = _clientID[1];
        oper.roomId = 1;
        oper.oper = 1;//check out
        oper.time = _sysTime;
        database::getInstance()->insertCheck(oper);
    }
}

void Server::on_check3_clicked()
{
    if(ui->check3->text()=="Check in"){
        _clientID[2]=_nextClientID;
        _nextClientID++;
        ui->Room3->setEnabled(true);
        ui->paylist3->setEnabled(false);
        ui->detail3->setEnabled(false);
        rooms[2].id->setText(QString::number(_clientID[2],10));
        rooms[2].roomNum->setText("2");
        rooms[2].Kwh->setText("0");
        rooms[2].fee->setText("0");
        ui->check3->setText("Check out");

        //数据库写入
        operation oper;
        oper.customerId = _clientID[2];
        oper.roomId = 2;
        oper.oper = 0;//check in
        oper.time = _sysTime;
        database::getInstance()->insertCheck(oper);
    }
    else{
        ui->paylist3->setEnabled(true);
        ui->detail3->setEnabled(true);
        ui->check3->setText("Check in");
        rooms[2].status->setText(QString::fromLocal8Bit("关机"));

        for(QList<ClientBlock*>::iterator it = _queue.begin(); it != _queue.end(); it++)
            if((*it)->getRoomNum() == 2)
            {
                (*it)->_socket->disconnectFromHost();
                delete (*it);
                _queue.erase(it);
                //数据库写入
                operation oper;
                oper.customerId = _clientID[2];
                oper.roomId = 2;
                oper.oper = 1;//关机
                oper.time = _sysTime;
                database::getInstance()->insertPower(oper);
                break;
            }

        //数据库写入
        operation oper;
        oper.customerId = _clientID[2];
        oper.roomId = 2;
        oper.oper = 1;//check out
        oper.time = _sysTime;
        database::getInstance()->insertCheck(oper);
    }
}

void Server::on_check4_clicked()
{
    if(ui->check4->text()=="Check in"){
        _clientID[3]=_nextClientID;
        _nextClientID++;
        ui->Room4->setEnabled(true);
        ui->paylist4->setEnabled(false);
        ui->detail4->setEnabled(false);
        rooms[3].id->setText(QString::number(_clientID[3],10));
        rooms[3].roomNum->setText("3");
        rooms[3].Kwh->setText("0");
        rooms[3].fee->setText("0");
        ui->check4->setText("Check out");

        //数据库写入
        operation oper;
        oper.customerId = _clientID[3];
        oper.roomId = 3;
        oper.oper = 0;//check in
        oper.time = _sysTime;
        database::getInstance()->insertCheck(oper);
    }
    else{
        ui->paylist4->setEnabled(true);
        ui->detail4->setEnabled(true);
        ui->check4->setText("Check in");
        rooms[3].status->setText(QString::fromLocal8Bit("关机"));

        for(QList<ClientBlock*>::iterator it = _queue.begin(); it != _queue.end(); it++)
            if((*it)->getRoomNum() == 3)
            {
                (*it)->_socket->disconnectFromHost();
                delete (*it);
                _queue.erase(it);
                //数据库写入
                operation oper;
                oper.customerId = _clientID[3];
                oper.roomId = 3;
                oper.oper = 1;//关机
                oper.time = _sysTime;
                database::getInstance()->insertPower(oper);
                break;
            }

        //数据库写入
        operation oper;
        oper.customerId = _clientID[3];
        oper.roomId = 3;
        oper.oper = 1;//check out
        oper.time = _sysTime;
        database::getInstance()->insertCheck(oper);
    }
}

void Server::on_onButton_clicked()
{
    ui->onButton->setEnabled(false);
    ui->offButton->setEnabled(true);
    ui->check1->setEnabled(true);
    ui->check2->setEnabled(true);
    ui->check3->setEnabled(true);
    ui->check4->setEnabled(true);
    //时间
    ui->year->setEnabled(true);
    ui->month->setEnabled(true);
    ui->day->setEnabled(true);
    ui->hour->setEnabled(true);
    ui->min->setEnabled(true);
    ui->sec->setEnabled(true);
    QDateTime Date=QDateTime::currentDateTime();
    QTime time=QTime::currentTime();
    _Year=Date.date().year();
    _Month=Date.date().month();
    _Day=Date.date().day();
    _Hour=time.hour();
    _Min=time.minute();
    _t = time.second();
    _timer->start(1000);
}

void Server::onTimeOut()
{
    _t++;
    if(_t == 60){
        _t = 0;
        _Min++;
        if(_Min==60){
            _Min=0;
            _Hour++;
            if(_Hour==24){
                _Hour=0;
                _Day++;
            }
        }
    }
    ui->sec->display(_t);
    ui->year->display(_Year);
    ui->month->display(_Month);
    ui->day->display(_Day);
    ui->hour->display(_Hour);
    ui->min->display(_Min);

    _sysTime = QString::number(_Year);
    _sysTime += "-" + QString::number(_Month);
    _sysTime += "-" + QString::number(_Day);
    _sysTime += " " + QString::number(_Hour);
    _sysTime += ":" + QString::number(_Min);
    _sysTime += ":" + QString::number(_t);

    qDebug() << "time out!";
    for(ClientBlock *client : _queue)
    {
        client->setSysTime(_sysTime);
        client->check();
    }
    schedule();
}

void Server::updateUI(ClientBlock* client)
{
    //qDebug() << "更新UI！！！";
    int i = client->getRoomNum();
    rooms[i].roomTmp->setText(QString::number(qRound(client->getRoomTmp())));
    rooms[i].targetTmp->setText(QString::number(qRound(client->getTargetTmp())));
    switch (client->getWindSpeed())
    {
    case 0:
        rooms[i].windSpeed->setText(QString::fromLocal8Bit("低风速"));
        break;
    case 1:
        rooms[i].windSpeed->setText(QString::fromLocal8Bit("中风速"));
        break;
    case 2:
        rooms[i].windSpeed->setText(QString::fromLocal8Bit("高风速"));
        break;
    default:
        break;
    }
    rooms[i].fee->setText(QString::number(client->getFee(), 10, 2));
    rooms[i].Kwh->setText(QString::number(client->getKwh(), 10, 2));
    if(client->isServed())
        rooms[i].status->setText(QString::fromLocal8Bit("服务"));
    else
        rooms[i].status->setText(QString::fromLocal8Bit("挂起"));
}

void Server::checkIsCheckedIn(ClientBlock* client)
{
    int i = client->getRoomNum();
    if(rooms[i].check->text() == "Check in")
    {
        client->setIsCheckedIn(false);
    }
    else
        client->setID(rooms[i].id->text().toInt());
}

void Server::on_okButton_clicked()
{
    //保证缺省目标温度在温控范围内
    if(ui->highestTmpEdit->text().toInt() >= ui->targetTmpEdit->text().toInt()
       && ui->lowestTmpEdit->text().toInt() <= ui->targetTmpEdit->text().toInt())
    {
        //制冷模式下的温控范围为18-25
        if(ui->coolRadio->isChecked()
           && ui->highestTmpEdit->text().toInt() <= 25
           && ui->lowestTmpEdit->text().toInt() >= 18)
        {
            ui->configBox->setEnabled(false);
            ui->onButton->setEnabled(true);
            ui->offButton->setEnabled(true);
        }
        //制热模式下的温控范围为25-30
        else if(ui->heatRadio->isChecked()
              && ui->highestTmpEdit->text().toInt() <= 30
              && ui->lowestTmpEdit->text().toInt() >= 25)
        {
            ui->configBox->setEnabled(false);
            ui->onButton->setEnabled(true);
            ui->offButton->setEnabled(true);
        }
    }
}

void Server::on_offButton_clicked()
{
    ui->paylist1->setEnabled(false);
    ui->detail1->setEnabled(false);
    ui->paylist2->setEnabled(false);
    ui->detail2->setEnabled(false);
    ui->paylist3->setEnabled(false);
    ui->detail3->setEnabled(false);
    ui->paylist4->setEnabled(false);
    ui->detail4->setEnabled(false);

    ui->check1->setEnabled(false);
    ui->check2->setEnabled(false);
    ui->check3->setEnabled(false);
    ui->check4->setEnabled(false);
    //房间监控清空
    for(int i=0;i<4;i++){
        rooms[i].id->clear();
        rooms[i].roomNum->clear();
        rooms[i].targetTmp->clear();
        rooms[i].fee->clear();
        rooms[i].windSpeed->clear();
        rooms[i].status->clear();
        rooms[i].roomTmp->clear();
        rooms[i].Kwh->clear();
    }
    //开关机
    ui->onButton->setEnabled(false);
    ui->offButton->setEnabled(false);
    ui->configBox->setEnabled(true);

    _timer->stop();
    ui->year->setEnabled(false);
    ui->month->setEnabled(false);
    ui->day->setEnabled(false);
    ui->hour->setEnabled(false);
    ui->min->setEnabled(false);
    ui->sec->setEnabled(false);
}

void Server::on_paylist1_clicked()
{
    ui->output->setText(database::getInstance()->getBill(ui->id1->text().toInt()));
}

void Server::on_paylist2_clicked()
{
    ui->output->setText(database::getInstance()->getBill(ui->id2->text().toInt()));
}

void Server::on_paylist3_clicked()
{
    ui->output->setText(database::getInstance()->getBill(ui->id3->text().toInt()));
}

void Server::on_paylist4_clicked()
{
    ui->output->setText(database::getInstance()->getBill(ui->id4->text().toInt()));
}

void Server::on_detail1_clicked()
{
    ui->output->setText(database::getInstance()->getDetailBill(ui->id1->text().toInt()));
}

void Server::on_detail2_clicked()
{
    ui->output->setText(database::getInstance()->getDetailBill(ui->id2->text().toInt()));
}

void Server::on_detail3_clicked()
{
    ui->output->setText(database::getInstance()->getDetailBill(ui->id3->text().toInt()));
}

void Server::on_detail4_clicked()
{
    ui->output->setText(database::getInstance()->getDetailBill(ui->id4->text().toInt()));
}

void Server::schedule(){//1.遍历queue把服务完成的从机放到队尾；2.调用qSort对服务未完成的从机排序（选前三），依据优先级
    //计数器，变为0时进行调度
    static int cnt = 10;
    if(cnt == 0){
        //服务未完成的放入queue_schedule，已完成的放入queue_satisfied
        QList<ClientBlock*> queue_schedule, queue_satisfied;
        for (int i = 0;i != _queue.size();i++){
            if(_queue.at(i)->isSatisfied() == false)
                queue_schedule.append(_queue.at(i));
            else
                queue_satisfied.append(_queue.at(i));
        }
        qDebug() << "queue length:" << queue_schedule.size();
        for (int i = 0; i != queue_schedule.size(); i++)
        {
            qDebug() << "room number:" <<queue_schedule[i]->getRoomNum();
            qDebug() << "isServed:" <<queue_schedule[i]->isServed();
            qDebug() << "isSatisfied:" <<queue_schedule[i]->isSatisfied();
        }
        //若queue_schedule中有超过3个从机，根据优先级排序
        if(queue_schedule.size() > 3){
            sortByWindSpeed(queue_schedule);
        }
        //将queue_schedule中最多前三个从机服务
        for(int i = 0;i != qMin(3,queue_schedule.size());i++){
            if(queue_schedule.at(i)->isServed() == false){
                queue_schedule.at(i)->setIsServed(true);
                qDebug() << "isServed:" << queue_schedule[i]->isServed();
                queue_schedule.at(i)->sendMessage();
            }
        }
        //剩下的挂起
        for(int i = 3;i < queue_schedule.size();i++){
            if(queue_schedule.at(i)->isServed() == true){
                queue_schedule.at(i)->setIsServed(false);
                queue_schedule.at(i)->sendMessage();
            }
        }
        _queue.clear();
        _queue = queue_schedule + queue_satisfied;
        cnt = 10;

		//更新UI
		for (int i = 0; i < _queue.size(); i++) {
            if(_queue[i]->isServed())
                rooms[i].status->setText(QString::fromLocal8Bit("服务"));
            else
                rooms[i].status->setText(QString::fromLocal8Bit("挂起"));
		}
		
    }else cnt --;
    return;
}

void Server::sortByWindSpeed(QList<ClientBlock*> &queue)
{
    //冒泡排序
    for(int i = queue.size() - 1; i > 0; i--)
        for(int j = 0; j < i; j++)
            if(queue[j]->getPriority() > queue[j + 1]->getPriority())
            {
                ClientBlock* temp = queue[j];
                queue[j] = queue[j + 1];
                queue[j + 1] = temp;
            }
}


