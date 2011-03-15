#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QLCDNumber>

#include "ledmeter.h"

enum Action { Launch, Thrust, RotateLeft, RotateRight, Shoot, Teleport,
                Brake, Shield, Pause, NewGame  };

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = 0);
    int shipsRemain;
    int score;
    bool waitShip;

    bool rotateL;
    bool rotateR;
    bool thrustShip;
    bool shootShip;
    bool teleportShip;
    bool brakeShip;
    bool pauseShip;
    bool shieldOn;

    int shotsFired;
    int shotsHit;
    int shootDelay;

    int mBrakeCount;
    int mShieldCount;
    int mTeleportCount;
    int mShootCount;

    bool vitalsChanged;

    int  shipAngle;
    int  rotateSlow;
    int  rotateRate;
    int  shipPower;

    QLCDNumber *scoreLCD;
    QLCDNumber *shipsLCD;

    QLCDNumber *teleportsLCD;
//    QLCDNumber *bombsLCD;
    QLCDNumber *brakesLCD;
    QLCDNumber *shieldLCD;
    QLCDNumber *shootLCD;
    KALedMeter *powerMeter;

    QMap<int,Action> actions;

signals:

public slots:

};

#endif // PLAYER_H
