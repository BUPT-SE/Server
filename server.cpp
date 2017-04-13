#include "server.h"
#include "ui_server.h"
#include "qmath.h"

Server::Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Server)
{
    ui->setupUi(this);
}

Server::~Server()
{
    delete ui;
}

void Server::on_check1_clicked()
{
    ui->Room1->setEnabled("true");

    //缺省值
}

void Server::on_check2_clicked()
{
    ui->Room2->setEnabled("true");
}


void Server::on_check3_clicked()
{
    ui->Room3->setEnabled("true");
}


void Server::on_check4_clicked()
{
    ui->Room4->setEnabled("true");
}


void Server::on_pushButton_clicked()
{
    ui->Room1->setEnabled("false");
    ui->Room2->setEnabled("false");
    ui->Room3->setEnabled("false");
    ui->Room4->setEnabled("false");
    ui->r1->setText("1");
    ui->r2->setText("2");
    ui->r3->setText("3");
    ui->r4->setText("4");
    ui->s1->setText("中速");
    ui->s2->setText("中速");
    ui->s3->setText("中速");
    ui->s4->setText("中速");
// 其他默认值

    ui->default2->setEnabled("false");
    //时间
    ui->yeart->display(year);
    ui->montht->display(month);
    ui->dayt->display(day);
    ui->hour->display(hour);
    ui->min->display(min);
}

void Server::schedule(){//1.遍历queue把服务完成的从机放到队尾；2.调用qSort对服务未完成的从机排序（选前三），依据优先级
    static int cnt = 10;
    if(cnt == 0){
        QList<ClientBlock*> queue_schedule,queue_satisfied;
        for (int i = 0;i != _queue.size();i++){
            if(_queue.at(i)->isSatisfied() == false)
                queue_schedule.append(_queue.at(i));
            else
                queue_satisfied.append(_queue.at(i));
        }
        if(queue_schedule.size() > 4){
            qsort(queue_schedule.begin(),queue_schedule.end(),compareSpeed);
        }
        for(int i = 0;i != qMin(4,queue_schedule.size());i++){
            if(queue_schedule.at(i)->getAttribute().getIsServed() == false){
                queue_schedule.at(i)->getAttribute().setIsServed(true);
                queue_schedule.at(i)->sendMessage();
            }
        }
        for(int i = 3;i < queue_schedule.size();i++){
            if(queue_schedule.at(i)->getAttribute().getIsServed() == true){
                queue_schedule.at(i)->getAttribute().setIsServed() == false;
                queue_schedule.at(i)->sendMessage();
            }
        }
        _queue.clear();
        _queue = queue_schedule + queue_satisfied;
        cnt = 10;
    }else cnt --;
    return;
}

bool Server::compareSpeed(const ClientBlock* x,const ClientBlock* y){
    if(x->getAttribute().getWindSpeed() > y->getAttribute().getWindSpeed()) return true;
    return false;
}
