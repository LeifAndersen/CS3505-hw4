#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QLCDNumber>

#include "ledmeter.h"

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = 0);
    int shipsRemain;
    int score;

    QLCDNumber *scoreLCD;
    QLCDNumber *shipsLCD;

    QLCDNumber *teleportsLCD;
//    QLCDNumber *bombsLCD;
    QLCDNumber *brakesLCD;
    QLCDNumber *shieldLCD;
    QLCDNumber *shootLCD;
    KALedMeter *powerMeter;
signals:

public slots:

};

#endif // PLAYER_H
