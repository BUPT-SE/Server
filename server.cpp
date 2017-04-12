#include "server.h"
#include "ui_server.h"

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

void Server::removeBlock(ClientBlock* blockptr)
{
    int pos;
    for(pos = 0; pos != _queue.size(); pos++)
    {
        if (_queue.at(i) == blockptr)
        {
            delete blockptr;
            break;
        }
    }
    if (i != _queue.size())
    {
        _queue.removeAt(i);
    }
}
