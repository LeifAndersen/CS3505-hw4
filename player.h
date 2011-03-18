#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QLCDNumber>

#include "ledmeter.h"
#include "sprites.h"

#define REFRESH_DELAY           33
#define SHIP_SPEED              0.3
#define MISSILE_SPEED           10.0
#define SHIP_STEPS              64
#define ROTATE_RATE             2
#define SHIELD_ON_COST          1
#define SHIELD_HIT_COST         30
#define BRAKE_ON_COST           4

enum Action { Launch, Thrust, RotateLeft, RotateRight, Shoot, Teleport,
                Brake, Shield, Pause, NewGame  };

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = 0);

    int shots() const { return shotsFired; }
    int hits() const { return shotsHit; }
    int power() const { return shipPower; }
    void shoot(bool s) {shootShip = s; shootDelay = 0; }
    void reducePower(int val);

    // Fields
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

    KShield *shield;
    AnimatedPixmapItem *ship;

    double shipDx;
    double shipDy;

    int shotsFired;
    int shotsHit;
    int shootDelay;

signals:

public slots:

};

#endif // PLAYER_H
