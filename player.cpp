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
