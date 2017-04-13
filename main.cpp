#include "server.h"
#include "database.h"
#include <QApplication>

database* database::instance=NULL;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Server w;
    w.show();
    database* databaseSingleton = database::getInstance();
    return a.exec();
}
