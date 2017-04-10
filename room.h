#ifndef ROOMS_H
#define ROOMS_H

#include<QLabel>

struct room{
    QLabel* id;
    QLabel* roomNum;
    QLabel* targetTmp;
    QLabel* roomTmp;
    QLabel* windSpeed;
    QLabel* status;
    QLabel* Kwh;
    QLabel* fee;
};

#endif // ROOMS_H
