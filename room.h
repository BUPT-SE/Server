#ifndef ROOMS_H
#define ROOMS_H

#include <QLabel>
#include <QPushButton>

struct room{
    QLabel* id;
    QLabel* roomNum;
    QLabel* targetTmp;
    QLabel* roomTmp;
    QLabel* windSpeed;
    QLabel* status;
    QLabel* Kwh;
    QLabel* fee;
    QPushButton* check;
};

#endif // ROOMS_H
