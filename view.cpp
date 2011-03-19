/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions
** contained in the either Technology Preview License Agreement or the
** Beta Release License Agreement.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
** $QT_END_LICENSE$
**
****************************************************************************/

/*
 * KAsteroids - Copyright (c) Martin R. Jones 1997
 *
 * Part of the KDE project
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

#include "view.h"

#define IMG_BACKGROUND ":/trolltech/examples/graphicsview/portedasteroids/bg.png"

#define REFRESH_DELAY           33
#define SHIP_SPEED              0.3
#define MISSILE_SPEED           10.0
#define SHIP_STEPS              64
#define ROTATE_RATE             2
#define SHIELD_ON_COST          1
#define SHIELD_HIT_COST         30
#define BRAKE_ON_COST           4

#define MAX_ROCK_SPEED          2.5
#define MAX_POWERUP_SPEED       1.5
#define MAX_SHIP_SPEED		12
#define MAX_BRAKES              5
#define MAX_SHIELDS             5
#define MAX_FIREPOWER		5

#define TEXT_SPEED              4

#define PI_X_2                  6.283185307
#ifndef M_PI
#define M_PI 3.141592654
#endif

static struct
{
    int id;
    const char *path;
    int frames;
}
kas_animations [] =
{
    { ID_ROCK_LARGE,       "rock1/rock1%1.png",       32 },
    { ID_ROCK_MEDIUM,      "rock2/rock2%1.png",       32 },
    { ID_ROCK_SMALL,       "rock3/rock3%1.png",       32 },
    { ID_SHIP,             "ship/ship%1.png",         32 },
    { ID_MISSILE,          "missile/missile.png",      1 },
    { ID_BIT,              "bits/bits%1.png",         16 },
    { ID_EXHAUST,          "exhaust/exhaust.png",      1 },
    { ID_ENERGY_POWERUP,   "powerups/energy.png",      1 },
    //    { ID_TELEPORT_POWERUP, "powerups/teleport%1.png", 12 },
    { ID_BRAKE_POWERUP,    "powerups/brake.png",       1 },
    { ID_SHIELD_POWERUP,   "powerups/shield.png",      1 },
    { ID_SHOOT_POWERUP,    "powerups/shoot.png",       1 },
    { ID_SHIELD,           "shield/shield%1.png",      6 },
    { 0,                   0,                          0 }
};

KAsteroidsView::KAsteroidsView(Player *player1, Player *player2,
                               QWidget *parent, const char *name)
                                   : QWidget( parent, name ),
                                   field(0, 0, 942, 690),
                                   view(&field,this)
{
    // Set the players
    this->player1 = player1;
    this->player2 = player2;

    // Set up the window
    view.setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    view.setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    view.setCacheMode(QGraphicsView::CacheBackground);
    view.setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    view.setOptimizationFlags(QGraphicsView::DontClipPainter
                              | QGraphicsView::DontSavePainterState
                              | QGraphicsView::DontAdjustForAntialiasing);
    view.viewport()->setFocusProxy( this );
    rocks.setAutoDelete( TRUE );
    player1->missiles.setAutoDelete( TRUE );
    player2->missiles.setAutoDelete( TRUE );
    bits.setAutoDelete( TRUE );
    powerups.setAutoDelete( TRUE );
    exhaust.setAutoDelete( TRUE );

    QPixmap pm( IMG_BACKGROUND );
    field.setBackgroundBrush( pm );

    textSprite = new QGraphicsTextItem( 0, &field );
    QFont font( "helvetica", 18 );
    textSprite->setFont( font );
    textSprite->setCacheMode(QGraphicsItem::DeviceCoordinateCache);

    player1->shield = 0;
    player2->shield = 0;

    player1->shieldOn = FALSE;
    player2->shieldOn = FALSE;
    refreshRate = REFRESH_DELAY;

    initialized = readSprites();
    player1->initialized = initialized;
    player2->initialized = initialized;

    mTimerId = -1;

    player1->shipPower = MAX_POWER_LEVEL;
    player2->shipPower = MAX_POWER_LEVEL;

    player1->vitalsChanged = TRUE;
    player2->vitalsChanged = TRUE;
    can_destroy_powerups = FALSE;

    mPaused = TRUE;

    if ( !initialized ) {
        textSprite->setHtml( tr("<font color=red>Error: Cannot read sprite images</font>") );
        textSprite->setPos( (field.width()-textSprite->boundingRect().width()) / 2,
                            (field.height()-textSprite->boundingRect().height()) / 2 );
    }
}

// - - -

KAsteroidsView::~KAsteroidsView()
{
}

// - - -

void KAsteroidsView::reset()
{
    if ( !initialized )
        return;
    rocks.clear();
    player1->missiles.clear();
    player2->missiles.clear();
    bits.clear();
    powerups.clear();
    exhaust.clear();

    player1->shotsFired = 0;
    player2->shotsFired = 0;

    player1->shotsHit = 0;
    player2->shotsHit = 0;

    rockSpeed = 1.0;
    powerupSpeed = 1.0;
    mFrameNum = 0;
    mPaused = FALSE;

    player1->ship->hide();
    player1->shield->hide();

    player1->ship->hide();
    player1->shield->hide();

    /*
    if ( mTimerId >= 0 ) {
    killTimer( mTimerId );
    mTimerId = -1;
    }
*/
}

// - --

void KAsteroidsView::newGame()
{
    if(!initialized)
        return;
    if(player1->shieldOn) {
        player1->shield->hide();
        player1->shieldOn = FALSE;
    }
    if(player2->shieldOn) {
        player2->shield->hide();
        player2->shieldOn = FALSE;
    }

    reset();
    if ( mTimerId < 0 )
        mTimerId = startTimer( REFRESH_DELAY );
    emit updateVitals();
}

// - - -

void KAsteroidsView::endGame()
{
}

void KAsteroidsView::pause( bool p )
{
    if ( !initialized )
        return;
    if ( !mPaused && p ) {
        if ( mTimerId >= 0 ) {
            killTimer( mTimerId );
            mTimerId = -1;
        }
    } else if ( mPaused && !p )
        mTimerId = startTimer( REFRESH_DELAY );
    mPaused = p;
}

// - - -

bool KAsteroidsView::readSprites()
{
    QString sprites_prefix = ":/trolltech/examples/graphicsview/portedasteroids/sprites/";

    int i = 0;
    while ( kas_animations[i].id )
    {
        QList<QPixmap> anim;
        QString wildcard = sprites_prefix + kas_animations[i].path;
        wildcard.replace("%1", "*");
        QFileInfo fi(wildcard);
        foreach (QString entry, QDir(fi.path(), fi.fileName()).entryList())
            anim << QPixmap(fi.path() + "/" + entry);
        animation.insert( kas_animations[i].id, anim );
        i++;
    }

    player1->ship = new AnimatedPixmapItem( animation[ID_SHIP], &field );
    player1->ship->hide();

    player2->ship = new AnimatedPixmapItem( animation[ID_SHIP], &field );
    player2->ship->hide();

    player1->shield = new KShield( animation[ID_SHIELD], &field );
    player1->shield->hide();

    player2->shield = new KShield( animation[ID_SHIELD], &field );
    player2->shield->hide();

    return (!player1->ship->image(0).isNull()
            && !player1->shield->image(0).isNull()
            && !player2->ship->image(0).isNull()
            && !player2->shield->image(0).isNull());
}

// - - -

void KAsteroidsView::addRocks( int num )
{
    if ( !initialized )
        return;
    for ( int i = 0; i < num; i++ )
    {
        KRock *rock = new KRock( animation[ID_ROCK_LARGE], &field,
                                 ID_ROCK_LARGE, randInt(2), randInt(2) ? -1 : 1 );
        double dx = (2.0 - randDouble()*4.0) * rockSpeed;
        double dy = (2.0 - randDouble()*4.0) * rockSpeed;
        rock->setVelocity( dx, dy );
        rock->setFrame( randInt( rock->frameCount() ) );
        if ( dx > 0 )
        {
            if ( dy > 0 )
                rock->setPos( 5, 5 );
            else
                rock->setPos( 5, field.height() - 25 );
            rock->setFrame( 0 );
        }
        else
        {
            if ( dy > 0 )
                rock->setPos( field.width() - 25, 5 );
            else
                rock->setPos( field.width() - 25, field.height() - 25 );
            rock->setFrame( 0 );
        }
        rock->show();
        rocks.append( rock );
    }
}

// - - -

void KAsteroidsView::showText( const QString &text, const QColor &color, bool scroll )
{
    if ( !initialized )
        return;
    textSprite->setHtml( QString("<font color=#%1%2%3>%4</font>")
                         .arg(color.red(), 2, 16, QLatin1Char('0'))
                         .arg(color.green(), 2, 16, QLatin1Char('0'))
                         .arg(color.blue(), 2, 16, QLatin1Char('0'))
                         .arg(text) );
    Q_UNUSED(color);
    // ### Porting: no such thing textSprite->setColor( color );

    if ( scroll ) {
        textSprite->setPos( (field.width()-textSprite->boundingRect().width()) / 2,
                            -textSprite->boundingRect().height() );
        textDy = TEXT_SPEED;
    } else {
        textSprite->setPos( (field.width()-textSprite->boundingRect().width()) / 2,
                            (field.height()-textSprite->boundingRect().height()) / 2 );
        textDy = 0;
    }
    textSprite->show();
}

// - - -

void KAsteroidsView::hideText()
{
    textDy = -TEXT_SPEED;
}

// - - -

void KAsteroidsView::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    field.setSceneRect(0, 0, width()-4, height()-4);
    view.resize(width(),height());
}

// - - -

void KAsteroidsView::timerEvent(QTimerEvent *)
{
    field.advance();

    AnimatedPixmapItem *rock;

    // move rocks forward
    for ( rock = rocks.first(); rock; rock = rocks.next() ) {
        ((KRock *)rock)->nextFrame();
        wrapSprite( rock );
    }

    wrapSprite(player1->ship);
    wrapSprite(player2->ship);

    // check for missile collision with rocks.
    processMissiles(player1);
    processMissiles(player2);

    // these are generated when a ship explodes
    for ( KBit *bit = bits.first(); bit; bit = bits.next() )
    {
        if ( bit->expired() )
        {
            bits.removeRef( bit );
        }
        else
        {
            bit->growOlder();
            bit->setFrame( ( bit->frame()+1 ) % bit->frameCount() );
        }
    }

    for ( KExhaust *e = exhaust.first(); e; e = exhaust.next() )
        exhaust.removeRef( e );

    // move / rotate ship.
    // check for collision with a rock.
    processShip(player1);
    processShip(player2);

    // move powerups and check for collision with player and missiles
    processPowerups(player1);
    processPowerups(player2);

    if ( textSprite->isVisible() )
    {
        if ( textDy < 0 &&
             textSprite->boundingRect().y() <= -textSprite->boundingRect().height() ) {
            textSprite->hide();
        } else {
            textSprite->moveBy( 0, textDy );
        }

        if ( textSprite->sceneBoundingRect().y() > (field.height()-textSprite->boundingRect().height())/2 )
            textDy = 0;
    }

    if ( (player1->vitalsChanged || player2->vitalsChanged) && !(mFrameNum % 10) ) {
        emit updateVitals();
        player1->vitalsChanged = FALSE;
        player2->vitalsChanged = FALSE;
    }

    mFrameNum++;
}

void KAsteroidsView::wrapSprite( QGraphicsItem *s )
{
    int x = int(s->x() + s->boundingRect().width() / 2);
    int y = int(s->y() + s->boundingRect().height() / 2);

    if ( x > field.width() )
        s->setPos( s->x() - field.width(), s->y() );
    else if ( x < 0 )
        s->setPos( field.width() + s->x(), s->y() );

    if ( y > field.height() )
        s->setPos( s->x(), s->y() - field.height() );
    else if ( y < 0 )
        s->setPos( s->x(), field.height() + s->y() );
}

// - - -

void KAsteroidsView::rockHit(Player *p, AnimatedPixmapItem *hit)
{
    KPowerup *nPup = 0;
    int rnd = int(randDouble()*30.0) % 30;
    switch( rnd )
    {
    case 4:
    case 5:
        nPup = new KPowerup( animation[ID_ENERGY_POWERUP], &field,
                             ID_ENERGY_POWERUP );
        break;
    case 10:
        //        nPup = new KPowerup( animation[ID_TELEPORT_POWERUP], &field,
        //                             ID_TELEPORT_POWERUP );
        break;
    case 15:
        nPup = new KPowerup( animation[ID_BRAKE_POWERUP], &field,
                             ID_BRAKE_POWERUP );
        break;
    case 20:
        nPup = new KPowerup( animation[ID_SHIELD_POWERUP], &field,
                             ID_SHIELD_POWERUP );
        break;
    case 24:
    case 25:
        nPup = new KPowerup( animation[ID_SHOOT_POWERUP], &field,
                             ID_SHOOT_POWERUP );
        break;
    }
    if ( nPup )
    {
        double r = 0.5 - randDouble();
        nPup->setPos( hit->x(), hit->y() );
        nPup->setFrame( 0 );
        nPup->setVelocity( hit->xVelocity() + r, hit->yVelocity() + r );
        powerups.append( nPup );
    }

    if ( hit->type() == ID_ROCK_LARGE || hit->type() == ID_ROCK_MEDIUM )
    {
        // break into smaller rocks
        double addx[4] = { 1.0, 1.0, -1.0, -1.0 };
        double addy[4] = { -1.0, 1.0, -1.0, 1.0 };

        double dx = hit->xVelocity();
        double dy = hit->yVelocity();

        double maxRockSpeed = MAX_ROCK_SPEED * rockSpeed;
        if ( dx > maxRockSpeed )
            dx = maxRockSpeed;
        else if ( dx < -maxRockSpeed )
            dx = -maxRockSpeed;
        if ( dy > maxRockSpeed )
            dy = maxRockSpeed;
        else if ( dy < -maxRockSpeed )
            dy = -maxRockSpeed;

        AnimatedPixmapItem *nrock;

        for ( int i = 0; i < 4; i++ )
        {
            double r = rockSpeed/2 - randDouble()*rockSpeed;
            if ( hit->type() == ID_ROCK_LARGE )
            {
                nrock = new KRock( animation[ID_ROCK_MEDIUM], &field,
                                   ID_ROCK_MEDIUM, randInt(2), randInt(2) ? -1 : 1 );
                emit rockHit(p, 0);
            }
            else
            {
                nrock = new KRock( animation[ID_ROCK_SMALL], &field,
                                   ID_ROCK_SMALL, randInt(2), randInt(2) ? -1 : 1 );
                emit rockHit(p, 1);
            }

            nrock->setPos( hit->x(), hit->y() );
            nrock->setFrame( 0 );
            nrock->setVelocity( dx+addx[i]*rockSpeed+r, dy+addy[i]*rockSpeed+r );
            nrock->setFrame( randInt( nrock->frameCount() ) );
            rocks.append( nrock );
        }
    }
    else if ( hit->type() == ID_ROCK_SMALL )
        emit rockHit(p,2);
    rocks.removeRef( hit );
    if ( rocks.count() == 0 )
        emit rocksRemoved();
}

void KAsteroidsView::addExhaust( double x, double y, double dx,
                                 double dy, int count )
{
    for ( int i = 0; i < count; i++ )
    {
        KExhaust *e = new KExhaust( animation[ID_EXHAUST], &field );
        e->setPos( x + 2 - randDouble()*4, y + 2 - randDouble()*4 );
        e->setVelocity( dx, dy );
        exhaust.append( e );
    }
}

void KAsteroidsView::processMissiles(Player *p)
{
    KMissile *missile;

    // if a missile has hit a rock, remove missile and break rock into smaller
    // rocks or remove completely.
    Q3PtrListIterator<KMissile> it(p->missiles);

    for ( ; it.current(); ++it )
    {
        missile = it.current();
        missile->growOlder();

        if ( missile->expired() )
        {
            p->missiles.removeRef( missile );
            continue;
        }

        wrapSprite( missile );

        QList<QGraphicsItem *> hits = missile->collidingItems(Qt::IntersectsItemBoundingRect);
        QList<QGraphicsItem *>::Iterator hit;
        for ( hit = hits.begin(); hit != hits.end(); ++hit )
        {
            if ( (*hit)->type() >= ID_ROCK_LARGE &&
                 (*hit)->type() <= ID_ROCK_SMALL && (*hit)->collidesWithItem(missile) )
            {
                p->shotsHit++;
                rockHit(p, static_cast<AnimatedPixmapItem *>(*hit));
                p->missiles.removeRef( missile );
                break;
            }
        }
    }
}

// - - -

void KAsteroidsView::processShip(Player *p)
{
    if ( p->ship->isVisible() )
    {
        if ( p->shieldOn )
        {
            p->shield->show();
            p->reducePower(SHIELD_ON_COST);
            static int sf = 0;
            sf++;

            if ( sf % 2 )
                p->shield->setFrame( (p->shield->frame()+1) % p->shield->frameCount() );
            p->shield->setPos( p->ship->x() - 9, p->ship->y() - 9 );

            QList<QGraphicsItem *> hits = p->shield->collidingItems(Qt::IntersectsItemBoundingRect);
            QList<QGraphicsItem *>::Iterator it;
            for ( it = hits.begin(); it != hits.end(); ++it )
            {
                if ( (*it)->type() >= ID_ROCK_LARGE &&
                     (*it)->type() <= ID_ROCK_SMALL && (*it)->collidesWithItem(p->shield) )
                {
                    int factor;
                    switch ( (*it)->type() )
                    {
                    case ID_ROCK_LARGE:
                        factor = 3;
                        break;

                    case ID_ROCK_MEDIUM:
                        factor = 2;
                        break;

                    default:
                        factor = 1;
                    }

                    if ( factor > p->mShieldCount )
                    {
                        // shield not strong enough
                        p->shieldOn = FALSE;
                        break;
                    }
                    rockHit(p, static_cast<AnimatedPixmapItem *>(*it));
                    // the more shields we have the less costly
                    p->reducePower(factor * (SHIELD_HIT_COST - p->mShieldCount*2));
                }
            }
        }

        if ( !p->shieldOn )
        {
            p->shield->hide();
            QList<QGraphicsItem *> hits = p->ship->collidingItems(Qt::IntersectsItemBoundingRect);
            QList<QGraphicsItem *>::Iterator it;
            for ( it = hits.begin(); it != hits.end(); ++it )
            {
                if ( (*it)->type() >= ID_ROCK_LARGE &&
                     (*it)->type() <= ID_ROCK_SMALL && (*it)->collidesWithItem(p->ship))
                {
                    KBit *bit;
                    for ( int i = 0; i < 12; i++ )
                    {
                        bit = new KBit( animation[ID_BIT], &field );
                        bit->setPos( p->ship->x() + 5 - randDouble() * 10,
                                     p->ship->y() + 5 - randDouble() * 10 );
                        bit->setFrame( randInt(bit->frameCount()) );
                        bit->setVelocity( 1-randDouble()*2,
                                          1-randDouble()*2 );
                        bit->setDeath( 60 + randInt(60) );
                        bits.append( bit );
                    }
                    p->ship->hide();
                    p->shield->hide();
                    emit shipKilled(p);
                    break;
                }
            }
        }


        if ( p->rotateSlow )
            p->rotateSlow--;

        if ( p->rotateL )
        {
            p->shipAngle -= p->rotateSlow ? 1 : p->rotateRate;
            if ( p->shipAngle < 0 )
                p->shipAngle += SHIP_STEPS;
        }

        if ( p->rotateR )
        {
            p->shipAngle += p->rotateSlow ? 1 : p->rotateRate;
            if ( p->shipAngle >= SHIP_STEPS )
                p->shipAngle -= SHIP_STEPS;
        }

        double angle = p->shipAngle * PI_X_2 / SHIP_STEPS;
        double cosangle = cos( angle );
        double sinangle = sin( angle );

        if ( p->brakeShip )
        {
            p->thrustShip = FALSE;
            p->rotateL = FALSE;
            p->rotateR = FALSE;
            p->rotateRate = ROTATE_RATE;
            if ( fabs(p->shipDx) < 2.5 && fabs(p->shipDy) < 2.5 )
            {
                p->shipDx = 0.0;
                p->shipDy = 0.0;
                p->ship->setVelocity( p->shipDx, p->shipDy );
                p->brakeShip = FALSE;
            }
            else
            {
                double motionAngle = atan2( -p->shipDy, -p->shipDx );
                if ( angle > M_PI )
                    angle -= PI_X_2;
                double angleDiff = angle - motionAngle;
                if ( angleDiff > M_PI )
                    angleDiff = PI_X_2 - angleDiff;
                else if ( angleDiff < -M_PI )
                    angleDiff = PI_X_2 + angleDiff;
                double fdiff = fabs( angleDiff );
                if ( fdiff > 0.08 )
                {
                    if ( angleDiff > 0 )
                        p->rotateL = TRUE;
                    else if ( angleDiff < 0 )
                        p->rotateR = TRUE;
                    if ( fdiff > 0.6 )
                        p->rotateRate = p->mBrakeCount + 1;
                    else if ( fdiff > 0.4 )
                        p->rotateRate = 2;
                    else
                        p->rotateRate = 1;

                    if ( p->rotateRate > 5 )
                        p->rotateRate = 5;
                }
                else if ( fabs(p->shipDx) > 1 || fabs(p->shipDy) > 1 )
                {
                    p->thrustShip = TRUE;
                    // we'll make braking a bit faster
                    p->shipDx += cosangle/6 * (p->mBrakeCount - 1);
                    p->shipDy += sinangle/6 * (p->mBrakeCount - 1);
                    p->reducePower(BRAKE_ON_COST);
                    addExhaust( p->ship->x() + 20 - cosangle*22,
                                p->ship->y() + 20 - sinangle*22,
                                p->shipDx-cosangle, p->shipDy-sinangle,
                                p->mBrakeCount+1 );
                }
            }
        }

        if ( p->thrustShip )
        {
            // The ship has a terminal velocity, but trying to go faster
            // still uses fuel (can go faster diagonally - don't care).
            double thrustx = cosangle/4;
            double thrusty = sinangle/4;
            if ( fabs(p->shipDx + thrustx) < MAX_SHIP_SPEED )
                p->shipDx += thrustx;
            if ( fabs(p->shipDy + thrusty) < MAX_SHIP_SPEED )
                p->shipDy += thrusty;
            p->ship->setVelocity( p->shipDx, p->shipDy );
            p->reducePower(1);
            addExhaust( p->ship->x() + 20 - cosangle*20,
                        p->ship->y() + 20 - sinangle*20,
                        p->shipDx-cosangle, p->shipDy-sinangle, 3 );
        }

        p->ship->setFrame( p->shipAngle >> 1 );

        if ( p->shootShip )
        {
            if ( !p->shootDelay && (int)p->missiles.count() < p->mShootCount + 4 )
            {
                KMissile *missile = new KMissile( animation[ID_MISSILE], &field );
                missile->setPos( 21+p->ship->x()+cosangle*21,
                                 21+p->ship->y()+sinangle*21 );
                missile->setFrame( 0 );
                missile->setVelocity( p->shipDx + cosangle*MISSILE_SPEED,
                                      p->shipDy + sinangle*MISSILE_SPEED );
                p->missiles.append( missile );
                p->shotsFired++;
                p->reducePower(1);

                p->shootDelay = 5;
            }

            if ( p->shootDelay )
                p->shootDelay--;
        }

        if ( p->teleportShip )
        {
            int ra = qrand() % 10;
            if( ra == 0 )
                ra += qrand() % 20;
            int xra = ra * 60 + ( (qrand() % 20) * (qrand() % 20) );
            int yra = ra * 50 - ( (qrand() % 20) * (qrand() % 20) );
            p->ship->setPos( xra, yra );
        }

        p->vitalsChanged = TRUE;
    }
}

// - - -

void KAsteroidsView::processPowerups(Player *p)
{
    if ( !powerups.isEmpty() )
    {
        // if player gets the powerup remove it from the screen, if option
        // "Can destroy powerups" is enabled and a missile hits the powerup
        // destroy it

        KPowerup *pup;
        Q3PtrListIterator<KPowerup> it( powerups );

        for( ; it.current(); ++it )
        {
            pup = it.current();
            pup->growOlder();

            if( pup->expired() )
            {
                powerups.removeRef( pup );
                continue;
            }

            wrapSprite( pup );

            QList<QGraphicsItem *> hits = pup->collidingItems();
            QList<QGraphicsItem *>::Iterator it;
            for ( it = hits.begin(); it != hits.end(); ++it )
            {
                if ( (*it) == p->ship )
                {
                    switch( pup->type() )
                    {
                    case ID_ENERGY_POWERUP:
                        p->shipPower += 150;
                        if ( p->shipPower > MAX_POWER_LEVEL )
                            p->shipPower = MAX_POWER_LEVEL;
                        break;
                    case ID_TELEPORT_POWERUP:
                        p->mTeleportCount++;
                        break;
                    case ID_BRAKE_POWERUP:
                        if ( p->mBrakeCount < MAX_BRAKES )
                            p->mBrakeCount++;
                        break;
                    case ID_SHIELD_POWERUP:
                        if ( p->mShieldCount < MAX_SHIELDS )
                            p->mShieldCount++;
                        break;
                    case ID_SHOOT_POWERUP:
                        if ( p->mShootCount < MAX_FIREPOWER )
                            p->mShootCount++;
                        break;
                    }

                    powerups.removeRef( pup );
                    p->vitalsChanged = TRUE;
                }
                else if ( (*it) == p->shield )
                {
                    powerups.removeRef( pup );
                }
                else if ( (*it)->type() == ID_MISSILE )
                {
                    if ( can_destroy_powerups )
                    {
                        powerups.removeRef( pup );
                    }
                }
            }
        }
    }         // -- if( powerups.isEmpty() )
}

// - - -

double KAsteroidsView::randDouble()
{
    int v = qrand();
    return (double)v / (double)RAND_MAX;
}

int KAsteroidsView::randInt( int range )
{
    return qrand() % range;
}

void KAsteroidsView::showEvent( QShowEvent *e )
{
#if defined( QT_LICENSE_PROFESSIONAL )
    static bool wasThere = FALSE;

    if ( !wasThere ) {
        wasThere = TRUE;
        QMessageBox::information( this, tr("QGraphicsView demo"),
                                  tr("This game has been implemented using the QGraphicsView class.\n"
                                     "The QGraphicsView class is not part of the Light Platform Edition. Please \n"
                                     "contact Qt Software if you want to upgrade to the Full Platform Edition.") );
    }
#endif

    QWidget::showEvent( e );
}
