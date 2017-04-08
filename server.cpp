#include "server.h"
#include "ui_server.h"
#include <QTimer>
#include <QDateTime>

Server::Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Server)
{
    ui->setupUi(this);
    ui->paylist1->setEnabled(false);
    ui->detail1->setEnabled(false);
    ui->paylist2->setEnabled(false);
    ui->detail2->setEnabled(false);
    ui->paylist3->setEnabled(false);
    ui->detail3->setEnabled(false);
    ui->paylist4->setEnabled(false);
    ui->detail4->setEnabled(false);
    ui->pushButton_2->setEnabled(false);
    ui->check1->setEnabled(false);
    ui->check2->setEnabled(false);
    ui->check3->setEnabled(false);
    ui->check4->setEnabled(false);
    ui->Room1->setEnabled(false);
    ui->Room2->setEnabled(false);
    ui->Room3->setEnabled(false);
    ui->Room4->setEnabled(false);
    ui->hot->setEnabled(false);
    ui->cold->setEnabled(false);
}

Server::~Server()
{
    delete ui;
}
void Server::on_check1_clicked()
{
    if(ui->check1->text()=="Check in"){
        clientID[1]=nextClientID;
        nextClientID++;
        ui->paylist1->setEnabled(false);
        ui->detail1->setEnabled(false);
        ui->Room1->setEnabled(true);

        ui->ID1->setText(QString::number(clientID[1],10));
        ui->roomnum1->setText("1");

        ui->consum1->setText("0");
        ui->pay1->setText("0");

        /*for(QList<ClientBlock*>::iterator it=_queue.begin();it!=_queue.end();)
            if(_queue.at(it)->_attribute.getRoomNum()==1)
                attribute=_queue.at((it))->_attribute;

        if(attribute->getRoomTmp()!=ui->t1->text().toFloat()&&attribute->getRoomTmp()!=0)
            ui->curtem1->setText(QString::number(attribute->getRoomTmp()));
        else
            ui->curtem1->setText(ui->t2->text());*/
        ui->curtem1->setText(ui->t1->text());

        /*if(ui->speed1->text().operator ==("-"))
            ui->speed1->setText(ui->s1->text());
        else {
            if(attribute->getWindSpeed()==0)
                ui->speed1->setText("Low");
            else if(attribute->getWindSpeed()==1)
                ui->speed1->setText("Middle");
            else if(attribute->getWindSpeed()==2)
                ui->speed1->setText("High");
        }*/
        ui->speed1->setText(ui->s1->text());

        /*if(ui->goal1->text().operator ==("0"))
            attribute->setTargetTmp(ui->g1->text().toFloat());
        ui->goal1->setText(QString::number(attribute->getTargetTmp()));*/
        ui->goal1->setText(ui->g1->text());

        if(ui->cold->isChecked())
            ui->curstate1->setText("制冷");
        else
            ui->curstate1->setText("制热");
        ui->check1->setText("Check out");
    }
    else{
        ui->paylist1->setEnabled(true);
        ui->detail1->setEnabled(true);
        ui->ID1->setText("-");
        ui->roomnum1->setText("-");
        ui->consum1->setText("-");
        ui->pay1->setText("-");
        ui->curstate1->setText("-");
        ui->speed1->setText("-");
        ui->curtem1->setText("0");
        ui->goal1->setText("0");
        ui->Room1->setEnabled(false);
        ui->check1->setText("Check in");
    }
}
void Server::on_check2_clicked()
{
    if(ui->check2->text()=="Check in"){
        clientID[2]=nextClientID;
        nextClientID++;

        ui->paylist2->setEnabled(false);
        ui->detail2->setEnabled(false);
        ui->Room2->setEnabled(true);

        ui->ID2->setText(QString::number(clientID[2],10));
        ui->roomnum2->setText("2");

        //for(QList<ClientBlock*>::iterator it=_queue.begin();it!=_queue.end();)
            //if(_queue.at(it)->_attribute.getRoomNum()==2)
                //attribute=_queue.at((it))->_attribute;

        ui->consum2->setText("0");
        ui->pay2->setText("0");

        /*if(attribute->getRoomTmp()!=ui->t2->text().toFloat()&&attribute->getRoomTmp()!=0)
            ui->curtem2->setText(QString::number(attribute->getRoomTmp()));
        else
            ui->curtem2->setText(ui->t2->text());*/
        ui->curtem2->setText(ui->t2->text());

        /*if(ui->speed2->text().operator ==("-"))
            ui->speed2->setText(ui->s2->text());
        else {
            if(attribute->getWindSpeed()==0)
                ui->speed2->setText("Low");
            else if(attribute->getWindSpeed()==1)
                ui->speed2->setText("Middle");
            else if(attribute->getWindSpeed()==2)
                ui->speed2->setText("High");
        }*/
        ui->speed2->setText(ui->s2->text());

        /*if(ui->goal2->text().operator ==("0"))
            attribute->setTargetTmp(ui->g2->text().toFloat());
        ui->goal2->setText(QString::number(attribute->getTargetTmp()));*/
        ui->goal2->setText(ui->g2->text());

        if(ui->cold->isChecked())
            ui->curstate2->setText("制冷");
        else if(ui->hot->isChecked())
            ui->curstate2->setText("制热");
        ui->check2->setText("Check out");
    }

    else{
        ui->paylist2->setEnabled(true);
        ui->detail2->setEnabled(true);
        ui->ID2->setText("-");
        ui->roomnum2->setText("-");
        ui->consum2->setText("-");
        ui->pay2->setText("-");
        ui->curstate2->setText("-");
        ui->speed2->setText("-");
        ui->curtem2->setText("0");
        ui->goal2->setText("0");
        ui->Room2->setEnabled(false);
        ui->check2->setText("Check in");
    }

}
void Server::on_check3_clicked()
{
    if(ui->check3->text()=="Check in"){
        clientID[3]=nextClientID;
        nextClientID++;
        ui->paylist3->setEnabled(false);
        ui->detail3->setEnabled(false);
        ui->Room3->setEnabled(true);

        ui->ID3->setText(QString::number(clientID[3],10));
        ui->roomnum3->setText("3");
        ui->consum3->setText("0");
        ui->pay3->setText("0");
        /*for(QList<ClientBlock*>::iterator it=_queue.begin();it!=_queue.end();)
            if(_queue.at(it)->_attribute.getRoomNum()==1)
                attribute=_queue.at((it))->_attribute;
        if(attribute->getRoomTmp()!=ui->t3->text().toFloat()&&attribute->getRoomTmp()!=0)
            ui->curtem3->setText(QString::number(attribute->getRoomTmp()));
        else
            ui->curtem3->setText(ui->t3->text());*/
        ui->curtem3->setText(ui->t3->text());
/*
        if(ui->speed3->text().operator ==("-"))
            ui->speed3->setText(ui->s3->text());
        else {
            if(attribute->getWindSpeed()==0)
                ui->speed3->setText("Low");
            else if(attribute->getWindSpeed()==1)
                ui->speed3->setText("Middle");
            else if(attribute->getWindSpeed()==2)
                ui->speed3->setText("High");
        }*/
        ui->speed3->setText(ui->s3->text());

        /*if(ui->goal3->text().operator ==("0"))
            attribute->setTargetTmp(ui->g3->text().toFloat());
        ui->goal3->setText(QString::number(attribute->getTargetTmp()));*/
        ui->goal3->setText(ui->g3->text());

        if(ui->cold->isChecked())
            ui->curstate3->setText("制冷");
        else if(ui->hot->isChecked())
            ui->curstate3->setText("制热");
        ui->check3->setText("Check out");
    }

    else{
        ui->paylist3->setEnabled(true);
        ui->detail3->setEnabled(true);
        ui->ID3->setText("-");
        ui->roomnum3->setText("-");
        ui->consum3->setText("-");
        ui->pay3->setText("-");
        ui->speed3->setText("-");
        ui->curtem3->setText("0");
        ui->goal3->setText("0");
        ui->Room3->setEnabled(false);
        ui->curstate3->setText("-");
        ui->check3->setText("Check in");
    }
}


void Server::on_check4_clicked()
{
    if(ui->check4->text()=="Check in"){
        clientID[4]=nextClientID;
        nextClientID++;

        ui->paylist4->setEnabled(false);
        ui->detail4->setEnabled(false);
        ui->Room4->setEnabled(true);

        ui->ID4->setText(QString::number(clientID[4],10));
        ui->roomnum4->setText("4");
        /*for(QList<ClientBlock*>::iterator it=_queue.begin();it!=_queue.end();)
            if(_queue.at(it)->_attribute.getRoomNum()==1)
                attribute=_queue.at((it))->_attribute;*/
        ui->consum4->setText("0");
        ui->pay4->setText("0");
/*
        if(attribute->getRoomTmp()!=ui->t4->text().toFloat()&&attribute->getRoomTmp()!=0)
            ui->curtem4->setText(QString::number(attribute->getRoomTmp()));
        else
            ui->curtem4->setText(ui->t4->text());*/
        ui->curtem4->setText(ui->t4->text());
        /*if(ui->speed4->text().operator ==("-"))
            ui->speed4->setText(ui->s4->text());
        else {
            if(attribute->getWindSpeed()==0)
                ui->speed4->setText("Low");
            else if(attribute->getWindSpeed()==1)
                ui->speed4->setText("Middle");
            else if(attribute->getWindSpeed()==2)
                ui->speed4->setText("High");
        }*/
        ui->speed4->setText(ui->s4->text());
        /*if(ui->goal4->text().operator ==("0"))
            attribute->setTargetTmp(ui->g4->text().toFloat());
        ui->goal4->setText(QString::number(attribute->getTargetTmp()));*/
        ui->goal4->setText(ui->g4->text());

        if(ui->cold->isChecked())
            ui->curstate4->setText("制冷");
        else if(ui->hot->isChecked())
            ui->curstate4->setText("制热");
        ui->check4->setText("Check out");
    }

    else{
        ui->paylist4->setEnabled(true);
        ui->detail4->setEnabled(true);
        ui->ID4->setText("-");
        ui->roomnum4->setText("-");
        ui->consum4->setText("-");
        ui->pay4->setText("-");
        ui->curstate4->setText("-");
        ui->speed4->setText("-");
        ui->curtem4->setText("0");
        ui->goal4->setText("0");
        ui->Room4->setEnabled(false);
        ui->check4->setText("Check in");
    }
}

void Server::on_pushButton_clicked()
{
    ui->pushButton->setEnabled(false);
    ui->pushButton_2->setEnabled(true);
    ui->cold->setEnabled(true);
    ui->hot->setEnabled(true);
    ui->r1->setText("1");
    ui->r2->setText("2");
    ui->r3->setText("3");
    ui->r4->setText("4");
    ui->s1->setText("Middle");
    ui->s2->setText("Middle");
    ui->s3->setText("Middle");
    ui->s4->setText("Middle");
    ui->t1->setText("30");
    ui->t2->setText("29");
    ui->t3->setText("27");
    ui->t4->setText("31");
// 其他默认值

    ui->default2->setEnabled(false);
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
    //ui->year->display(Year);

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

void Server::on_pushButton_2_clicked()
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
    ui->ID1->setText("-");
    ui->roomnum1->setText("-");
    ui->consum1->setText("-");
    ui->pay1->setText("-");
    ui->speed1->setText("-");
    ui->curstate1->setText("-");
    ui->curtem1->setText("0");
    ui->goal1->setText("0");

    ui->ID2->setText("-");
    ui->roomnum2->setText("-");
    ui->consum2->setText("-");
    ui->pay2->setText("-");
    ui->speed2->setText("-");
    ui->curstate2->setText("-");
    ui->curtem2->setText("0");
    ui->goal2->setText("0");

    ui->ID3->setText("-");
    ui->roomnum3->setText("-");
    ui->consum3->setText("-");
    ui->pay3->setText("-");
    ui->speed3->setText("-");
    ui->curstate3->setText("-");
    ui->curtem3->setText("0");
    ui->goal3->setText("0");

    ui->ID4->setText("-");
    ui->roomnum4->setText("-");
    ui->consum4->setText("-");
    ui->pay4->setText("-");
    ui->speed4->setText("-");
    ui->curstate4->setText("-");
    ui->curtem4->setText("0");
    ui->goal4->setText("0");

    ui->Room1->setEnabled(false);
    ui->Room2->setEnabled(false);
    ui->Room3->setEnabled(false);
    ui->Room4->setEnabled(false);

//配置区还原
    ui->r1->setText("-");
    ui->r2->setText("-");
    ui->r3->setText("-");
    ui->r4->setText("-");
    ui->s1->setText("-");
    ui->s2->setText("-");
    ui->s3->setText("-");
    ui->s4->setText("-");
    ui->g1->setText("-");
    ui->g2->setText("-");
    ui->g3->setText("-");
    ui->g4->setText("-");
    ui->t1->setText("-");
    ui->t2->setText("-");
    ui->t3->setText("-");
    ui->t4->setText("-");
    //开关机
    ui->pushButton->setEnabled(true);
    ui->pushButton_2->setEnabled(false);
    //时间计时停止
    timer->stop();
    connect(timer,SIGNAL(timeout()),this,SLOT(onTimeOut()));
    ui->year->setEnabled(false);
    ui->month->setEnabled(false);
    ui->day->setEnabled(false);
    ui->hour->setEnabled(false);
    ui->min->setEnabled(false);
}

void Server::on_cold_clicked()
{
    ui->check1->setEnabled(true);
    ui->check2->setEnabled(true);
    ui->check3->setEnabled(true);
    ui->check4->setEnabled(true);
    ui->cold->setEnabled(false);
    ui->hot->setEnabled(false);
    //制冷目标温度

}

void Server::on_hot_clicked()
{
    ui->check1->setEnabled(true);
    ui->check2->setEnabled(true);
    ui->check3->setEnabled(true);
    ui->check4->setEnabled(true);
    ui->cold->setEnabled(false);
    ui->hot->setEnabled(false);
    //制热目标温度

}
