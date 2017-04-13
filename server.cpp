#include "server.h"
#include "ui_server.h"

Server::Server(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Server)
{
    ui->setupUi(this);

    //new server
    this->_server=new QTcpServer();
    //监听所有的连接请求
    if(!_server->listen(QHostAddress::Any,666))
    {
        qDebug() <<"Contect Error";
    }
    else
    {
       qDebug() <<"Contect Succeed";

    }

    connect(_server,SIGNAL(newConnection()),this,SLOT(newConnection()));

}

void Server::newConnection()
{
    ClientBlock* newClientBlock=new ClientBlock(_server->nextPendingConnection(),0);
    this->_queue.push_back(newClientBlock);
    qDebug()<<"queue length";
    connect(newClientBlock,SIGNAL(shutdown(ClientBlock*)),this,SLOT(disConnection(ClientBlock*)));
}

void Server::disConnection(ClientBlock* clientBlock)
{

    QList<ClientBlock*>::iterator it;
    qDebug()<<"queue length"<<_queue.size()<<endl;
    for(it=_queue.begin();it!=_queue.end();it++)
    {

        //如果找到了这个房间对应的block
        if(*it==clientBlock)
        {
            _queue.erase(it);
            break;
        }

    }
    qDebug()<<"queue length"<<_queue.size()<<endl;
    delete clientBlock;
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
