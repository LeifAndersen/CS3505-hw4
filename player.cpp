/* Leif Andersen, Robert Christensen
 * (c) TEAM: Katastrofie, 2011
 *
 * Released under the terms of the LGPL v2.1
 * as part of a modification of the KAstroids projec
 * (from the qt demo library), which is where most of
 * the code came from.
 */
#include <stdlib.h>
#include <math.h>
#include <qapplication.h>
#include <qnamespace.h>
#include <q3accel.h>
#include <qmessagebox.h>
#include <q3scrollview.h>
#include <qdir.h>
#include <QGraphicsItem>
//Added by qt3to4:
#include <QTimerEvent>
#include <QPixmap>
#include <QResizeEvent>
#include <QShowEvent>

#include "player.h"

/**
  *Constructs a player
  * Params: parent, the parent for the qobject, defaults to 0
  */
Player::Player(QObject *parent) : QObject(parent) {
    shieldTimer = new QTimer( this );
    connect( shieldTimer, SIGNAL(timeout()), this, SLOT(hideShield()) );
}

/**
  * Creates a new ship for the player.
  *
  * Params: Starting x coordinate for the ship.
  *         Starting y coordinate for the ship.
  */
void Player::newShip(int x, int y)
{
    if ( !initialized )
    return;
    ship->setPos( x, y );
    ship->setFrame( 0 );
    shield->setPos( x, y );
    shield->setFrame( 0 );
    ship->setVelocity( 0.0, 0.0 );
    shipDx = 0;
    shipDy = 0;
    shipAngle = 0;
    rotateL = FALSE;
    rotateR = FALSE;
    thrustShip = FALSE;
    shootShip = FALSE;
    brakeShip = FALSE;
    teleportShip = FALSE;
    shieldOn = TRUE;
    shootDelay = 0;
    shipPower = MAX_POWER_LEVEL;
    rotateRate = ROTATE_RATE;
    rotateSlow = 0;

    mBrakeCount = 0;
    mTeleportCount = 0;
    mShootCount = 0;

    ship->show();
    shield->show();
    mShieldCount = 1;   // just in case the ship appears on a rock.
    shieldTimer->start( 1000, TRUE );
}

/**
  * Hides the ship
   */
void Player::hideShield()
{
    shield->hide();
    mShieldCount = 0;
    shieldOn = FALSE;
}

/**
  * Reduces the power of the ship.
  */
void Player::reducePower(int val)
{
    shipPower -= val;
    if ( shipPower <= 0 )
    {
    shipPower = 0;
    thrustShip = FALSE;
    if ( shieldOn )
    {
        shieldOn = FALSE;
        shield->hide();
    }
    }
    vitalsChanged = TRUE;
}

/**
  * Set's the ship's shield, given power.
  */
void Player::setShield(bool s)
{
    if (!initialized)
        return;
    if (shieldTimer->isActive() && !s) {
        shieldTimer->stop();
        hideShield();
    } else {
        shieldOn = s && mShieldCount;
    }
}

/**
  * Breaks the ship, if the ship can be stopped.
  */
void Player::brake(bool b)
{
    if (!initialized)
        return;
    if (mBrakeCount) {
        if (brakeShip && !b) {
            rotateL = FALSE;
            rotateR = FALSE;
            thrustShip = FALSE;
            rotateRate = ROTATE_RATE;
        }
        brakeShip = b;
    }
}
