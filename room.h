#ifndef ROOMS_H
#define ROOMS_H

#include <QLabel>
#include <QPushButton>

struct Room{
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

struct Bill{
    double fee;
    double Kwh;
};

#endif // ROOMS_H
