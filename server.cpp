#include "server.h"
#include "ui_server.h"
#include "room.h"
#include "database.h"
#include <QTimer>
#include <QDateTime>
#include <QVector>

Server::Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Server)
{
    t=0;
    nextClientID=1;
    ui->setupUi(this);
    room room1,room2,room3,room4;
    room1.id=ui->id1;
    room1.roomTmp=ui->roomTmp1;
    room1.roomNum=ui->roomNum1;
    room1.targetTmp=ui->targetTmp1;
    room1.windSpeed=ui->windSpeed1;
    room1.status=ui->status1;
    room1.Kwh=ui->Kwh1;
    room1.fee=ui->fee1;

    room2.id=ui->id2;
    room2.roomTmp=ui->roomTmp2;
    room2.roomNum=ui->roomNum2;
    room2.targetTmp=ui->targetTmp2;
    room2.windSpeed=ui->windSpeed2;
    room2.status=ui->status2;
    room2.Kwh=ui->Kwh2;
    room2.fee=ui->fee2;

    room3.id=ui->id3;
    room3.roomTmp=ui->roomTmp3;
    room3.roomNum=ui->roomNum3;
    room3.targetTmp=ui->targetTmp3;
    room3.windSpeed=ui->windSpeed3;
    room3.status=ui->status3;
    room3.Kwh=ui->Kwh3;
    room3.fee=ui->fee3;

    room4.id=ui->id4;
    room4.roomTmp=ui->roomTmp4;
    room4.targetTmp=ui->targetTmp4;
    room4.windSpeed=ui->windSpeed4;
    room4.roomNum=ui->roomNum4;
    room4.status=ui->status4;
    room4.Kwh=ui->Kwh4;
    room4.fee=ui->fee4;

    rooms.append(room1);
    rooms.append(room2);
    rooms.append(room3);
    rooms.append(room4);

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

}

Server::~Server()
{
    delete ui;
}
void Server::on_check1_clicked()
{

    if(ui->check1->text()=="Check in"){
        clientID[0]=nextClientID;
        nextClientID++;
        ui->Room1->setEnabled(true);
        ui->paylist1->setEnabled(false);
        ui->detail1->setEnabled(false);

        rooms[0].id->setText(QString::number(clientID[0],10));
        rooms[0].roomNum->setText("0");
        rooms[0].Kwh->setText("0");
        rooms[0].fee->setText("0");
        ui->check1->setText("Check out");
    }
    else{
        ui->paylist1->setEnabled(true);
        ui->detail1->setEnabled(true);
        ui->check1->setText("Check in");
        ui->Room1->setEnabled(false);
    }
}
void Server::on_check2_clicked()
{
    if(ui->check2->text()=="Check in"){
        clientID[1]=nextClientID;
        nextClientID++;
        ui->Room2->setEnabled(true);
        ui->paylist2->setEnabled(false);
        ui->detail2->setEnabled(false);
        rooms[1].id->setText(QString::number(clientID[1],10));
        rooms[1].roomNum->setText("1");
        rooms[1].Kwh->setText("0");
        rooms[1].fee->setText("0");
        ui->check2->setText("Check out");
    }

    else{
        ui->paylist2->setEnabled(true);
        ui->detail2->setEnabled(true);
        ui->check2->setText("Check in");
    }

}
void Server::on_check3_clicked()
{
    if(ui->check3->text()=="Check in"){
        clientID[2]=nextClientID;
        nextClientID++;
        ui->Room3->setEnabled(true);
        ui->paylist3->setEnabled(false);
        ui->detail3->setEnabled(false);
        rooms[2].id->setText(QString::number(clientID[2],10));
        rooms[2].roomNum->setText("2");
        rooms[2].Kwh->setText("0");
        rooms[2].fee->setText("0");
        ui->check3->setText("Check out");
    }

    else{
        ui->paylist3->setEnabled(true);
        ui->detail3->setEnabled(true);
        ui->check3->setText("Check in");
    }
}


void Server::on_check4_clicked()
{
    if(ui->check4->text()=="Check in"){
        clientID[3]=nextClientID;
        nextClientID++;
        ui->Room4->setEnabled(true);
        ui->paylist4->setEnabled(false);
        ui->detail4->setEnabled(false);
        rooms[3].id->setText(QString::number(clientID[3],10));
        rooms[3].roomNum->setText("3");
        rooms[3].Kwh->setText("0");
        rooms[3].fee->setText("0");

        ui->check4->setText("Check out");
    }

    else{
        ui->paylist4->setEnabled(true);
        ui->detail4->setEnabled(true);
        ui->check4->setText("Check in");
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
    Year=Date.date().year();
    Month=Date.date().month();
    Day=Date.date().day();
    Hour=time.hour();
    Min=time.minute();
    timer->setInterval(2000);
    timer->start(1000);
    connect(timer,SIGNAL(timeout()),this,SLOT(onTimeOut()));
}
void Server::onTimeOut(){
    t++;
    if(t%10==0){
        Min++;
        if(Min==60){
            Min=0;
            Hour++;
            if(Hour==24){
                Hour=0;
                Day++;
            }
        }
    }
    ui->year->display(Year);
    ui->month->display(Month);
    ui->day->display(Day);
    ui->hour->display(Hour);
    ui->min->display(Min);
}

void Server::on_okButton_clicked()
{
    ui->configBox->setEnabled(false);
    ui->onButton->setEnabled(true);
    ui->offButton->setEnabled(true);
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

    timer->stop();
    connect(timer,SIGNAL(timeout()),this,SLOT(onTimeOut()));
    ui->year->setEnabled(false);
    ui->month->setEnabled(false);
    ui->day->setEnabled(false);
    ui->hour->setEnabled(false);
    ui->min->setEnabled(false);
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
