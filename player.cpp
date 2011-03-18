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

Player::Player(QObject *parent) : QObject(parent) {
    shieldTimer = new QTimer( this );
    connect( shieldTimer, SIGNAL(timeout()), this, SLOT(hideShield()) );
}


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


void Player::hideShield()
{
    shield->hide();
    mShieldCount = 0;
    shieldOn = FALSE;
}

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
