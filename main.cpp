#include "server.h"
#include "database.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Server w;
    w.show();
    
    database* databaseSingleton = database::getInstance();

    return a.exec();
}
