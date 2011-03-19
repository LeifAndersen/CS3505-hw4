/* Leif Andersen, Robert Christensen
 * (c) TEAM: Katastrofie, 2011
 *
 * Released under the terms of the LGPL v2.1
 * as part of a modification of the KAstroids projec
 * (from the qt demo library), which is where most of
 * the code came from.
 */
#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QLCDNumber>
#include <qwidget.h>
#include <q3ptrlist.h>
#include <q3intdict.h>
#include <qtimer.h>
#include <QGraphicsScene>
#include <QGraphicsView>
//Added by qt3to4:
#include <QTimerEvent>
#include <QShowEvent>
#include <QResizeEvent>

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
#define MAX_POWER_LEVEL          1000

/**
  * An enumeration of all of the possible actions
  * a player can take.  Should be mapped
  * to the player's kepmap outside of the player object itself.
  */
enum Action { Launch, Thrust, RotateLeft, RotateRight, Shoot, Teleport,
                Brake, Shield, Pause, NewGame  };

/**
  * A representation of a player.
  *
  * The reason why most of the data is public is due
  * to the original structure of the code, in which
  * the player data was baked right into the view.
  *
  * This method allows for many different players.
  */
class Player : public QObject
{
    Q_OBJECT
public:
    /**
      *Constructs a player
      * Params: parent, the parent for the qobject, defaults to 0
      */
    explicit Player(QObject *parent = 0);

    /**
      * Returns the shotsFired Variable
      */
    int shots() const { return shotsFired; }

    /**
      * Returns the shotsHit variable
      */
    int hits() const { return shotsHit; }

    /**
      * Returns the ship shippower variable
      */
    int power() const { return shipPower; }

    /**
      * Sets the rotateL variable, and speed to 5
      */
    void rotateLeft(bool r) { rotateL = r; rotateSlow = 5; }

    /**
      * Sets the rotateL variable, and speed to 5
      */
    void rotateRight(bool r) { rotateR = r; rotateSlow = 5; }

    /**
      * Sets the thrust of the ship, assuming power is greater than 0.
      */
    void thrust(bool t) { thrustShip = t && shipPower > 0; }

    /**
      * Set the telaport to the ship, assuming you can telaport.
      * (not used in current version)
      */
    void teleport(bool te) { teleportShip = te && mTeleportCount; }

    /**
      * Breaks the ship, if the ship can be stopped.
      */
    void brake(bool b);

    /**
      * Returns the mTeleportCount
      */
    int teleportCount() const { return mTeleportCount; }

    /**
      * Returns mBreakCount
      */
    int brakeCount() const { return mBrakeCount; }

    /**
      * Returns mShieldCount
      */
    int shieldCount() const { return mShieldCount; }

    /**
      * Returns mShootCount
      */
    int shootCount() const { return mShootCount; }

    /**
      * Set's the ship to shoot.
      */
    void shoot(bool s) {shootShip = s; shootDelay = 0; }

    /**
      * Reduces the power of the ship.
      */
    void reducePower(int val);

    /**
      * Set's the ship's shield, given power.
      */
    void setShield(bool s);

    /**
      * Creates a new ship for the player.
      *
      * Params: Starting x coordinate for the ship.
      *         Starting y coordinate for the ship.
      */
    void newShip(int x, int y);

    // Fields
    // The rest of this section is for fields
    // These fields only exist for more compatibility with
    // the view class.  Anyone who has free time should spend the time
    // to make as many of these as possible private.

    // Player stats
    int shipsRemain;
    int score;
    bool waitShip;

    //Moving ship
    bool rotateL;
    bool rotateR;
    bool thrustShip;
    bool shootShip;
    bool teleportShip;
    bool brakeShip;
    bool pauseShip;
    bool shieldOn;
    int  shipAngle;
    int  rotateSlow;
    int  rotateRate;
    int  shipPower;
    double shipDx;
    double shipDy;

    // Ship stats
    int mBrakeCount;
    int mShieldCount;
    int mTeleportCount;
    int mShootCount;
    int shotsFired;
    int shotsHit;
    int shootDelay;

    // Gui needs updating
    bool vitalsChanged;

    // GUI for the player (may be moved
    QLCDNumber *scoreLCD;
    QLCDNumber *shipsLCD;
    QLCDNumber *teleportsLCD;
//    QLCDNumber *bombsLCD;
    QLCDNumber *brakesLCD;
    QLCDNumber *shieldLCD;
    QLCDNumber *shootLCD;
    KALedMeter *powerMeter;

    /**
      * Actions for the player
      * currently must be set outside of this class
      */
    QMap<int,Action> actions;

    // Gui for the ship, must NOT be moved outside of class
    KShield *shield;
    AnimatedPixmapItem *ship;

    // Initialization of the ship, done outside of this class
    bool initialized;

    // Timer for the shield
    QTimer *shieldTimer;

    // Player's missiles on the field.
    Q3PtrList<KMissile> missiles;

private slots:
    /**
      * Hides the ship
       */
    void hideShield();
};

#endif // PLAYER_H
