#include "server.h"
#include "ui_server.h"

Server::Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Server),
    timer(new QTimer())
{
    ui->setupUi(this);
    connect(timer,QTimer::timeout,this,Server::timeout);
    timer->start(1000);
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

void Server::timeout()
{
    //系统时间加一秒
    for(ClientBlock *client:_queue)
    {
        client->check();
    }
    schedule();
}
