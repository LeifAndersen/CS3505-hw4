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
//	--- toplevel.cpp ---
#include <q3accel.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qlcdnumber.h>
#include <qpushbutton.h>

#include <qapplication.h>
//Added by qt3to4:
#include <Q3HBoxLayout>
#include <QShowEvent>
#include <Q3Frame>
#include <QPixmap>
#include <QHideEvent>
#include <QKeyEvent>
#include <Q3VBoxLayout>

#include "toplevel.h"
#include "ledmeter.h"
#include "settings.h"


#define SB_SCORE	1
#define SB_LEVEL	2
#define SB_SHIPS	3

struct SLevel
{
    int    nrocks;
    double rockSpeed;
};

#define MAX_LEVELS	16

SLevel levels[MAX_LEVELS] =
{
    { 2, 0.4 },
    { 2, 0.6 },
    { 3, 0.5 },
    { 3, 0.7 },
    { 3, 0.8 },
    { 6, 0.6 },
    { 6, 0.7 },
    { 6, 0.8 },
    { 8, 0.6 },
    { 8, 0.7 },
    { 8, 0.8 },
    { 10, 0.7 },
    { 10, 0.8 },
    { 10, 0.9 },
    { 10, 1.0 }
};

const char *soundEvents[] =
{
    "ShipDestroyed",
    "RockDestroyed",
    0
};

const char *soundDefaults[] =
{
    "Explosion.wav",
    "ploop.wav",
    0
};


KAstTopLevel::KAstTopLevel( QWidget *parent, const char *name )
    : Q3MainWindow( parent, name, 0 )
{

    // Set up the players
    player1 = new Player(this);
    player2 = new Player(this);

    QWidget *border = new QWidget( this );
    border->setBackgroundColor( Qt::black );
    setCentralWidget( border );

    Q3VBoxLayout *borderLayout = new Q3VBoxLayout( border );
    borderLayout->addStretch( 1 );

    QWidget *mainWin = new QWidget( border );
    mainWin->setFixedSize(942, 690);
    borderLayout->addWidget( mainWin, 0, Qt::AlignHCenter );

    borderLayout->addStretch( 1 );

    view = new KAsteroidsView(player1, player2, mainWin);
    view->setFocusPolicy( Qt::StrongFocus );
    connect( view, SIGNAL( shipKilled(Player *) ), SLOT( slotShipKilled(Player *) ) );
    connect( view, SIGNAL( rockHit(int) ), SLOT( slotRockHit(int) ) );
    connect( view, SIGNAL( rocksRemoved() ), SLOT( slotRocksRemoved() ) );
    connect( view, SIGNAL( updateVitals() ), SLOT( slotUpdateVitals() ) );

    Q3VBoxLayout *vb = new Q3VBoxLayout( mainWin );
    Q3HBoxLayout *hb = new Q3HBoxLayout;
    Q3HBoxLayout *hbd = new Q3HBoxLayout;
    vb->addLayout( hb );

    QFont labelFont( "helvetica", 24 );
    QColorGroup grp( Qt::darkGreen, Qt::black, QColor( 128, 128, 128 ),
        QColor( 64, 64, 64 ), Qt::black, Qt::darkGreen, Qt::black );
    QPalette pal( grp, grp, grp );

    mainWin->setPalette( pal );

    // Set up top bar
    hb->addSpacing( 10 );

    QLabel *label;
    label = new QLabel( tr("P1:Score"), mainWin );
    label->setFont( labelFont );
    label->setPalette( pal );
    label->setFixedWidth( label->sizeHint().width() );
    hb->addWidget( label );

    player1->scoreLCD = new QLCDNumber( 6, mainWin );
    player1->scoreLCD->setFrameStyle( Q3Frame::NoFrame );
    player1->scoreLCD->setSegmentStyle( QLCDNumber::Flat );
    player1->scoreLCD->setFixedWidth( 150 );
    player1->scoreLCD->setPalette( pal );
    hb->addWidget( player1->scoreLCD );
    hb->addStretch( 30 );

    label = new QLabel( tr("Ships"), mainWin );
    label->setFont( labelFont );
    label->setFixedWidth( label->sizeHint().width() );
    label->setPalette( pal );
    hb->addWidget( label );

    player1->shipsLCD = new QLCDNumber( 1, mainWin );
    player1->shipsLCD->setFrameStyle( Q3Frame::NoFrame );
    player1->shipsLCD->setSegmentStyle( QLCDNumber::Flat );
    player1->shipsLCD->setFixedWidth( 40 );
    player1->shipsLCD->setPalette( pal );
    hb->addWidget( player1->shipsLCD );

    hb->addStrut( 30 );
    hb->addStretch( 30 );

    label = new QLabel( tr("P2:Score"), mainWin );
    label->setFont( labelFont );
    label->setPalette( pal );
    label->setFixedWidth( label->sizeHint().width() );
    hb->addWidget( label );

    player2->scoreLCD = new QLCDNumber( 6, mainWin );
    player2->scoreLCD->setFrameStyle( Q3Frame::NoFrame );
    player2->scoreLCD->setSegmentStyle( QLCDNumber::Flat );
    player2->scoreLCD->setFixedWidth( 150 );
    player2->scoreLCD->setPalette( pal );
    hb->addWidget( player2->scoreLCD );
    hb->addStretch( 10 );

    label = new QLabel( tr("Ships"), mainWin );
    label->setFont( labelFont );
    label->setFixedWidth( label->sizeHint().width() );
    label->setPalette( pal );
    hb->addWidget( label );

    player2->shipsLCD = new QLCDNumber( 1, mainWin );
    player2->shipsLCD->setFrameStyle( Q3Frame::NoFrame );
    player2->shipsLCD->setSegmentStyle( QLCDNumber::Flat );
    player2->shipsLCD->setFixedWidth( 40 );
    player2->shipsLCD->setPalette( pal );
    hb->addWidget( player2->shipsLCD );

    hb->addStrut( 30 );
    hb->addStretch( 30 );

    label = new QLabel( tr("Level"), mainWin );
    label->setFont( labelFont );
    label->setPalette( pal );
    label->setFixedWidth( label->sizeHint().width() );
    hb->addWidget( label );

    levelLCD = new QLCDNumber( 2, mainWin );
    levelLCD->setFrameStyle( Q3Frame::NoFrame );
    levelLCD->setSegmentStyle( QLCDNumber::Flat );
    levelLCD->setFixedWidth( 70 );
    levelLCD->setPalette( pal );
    hb->addWidget( levelLCD );

    vb->addWidget( view, 10 );

// -- bottom layout:
    vb->addLayout( hbd );

    QFont smallFont( "helvetica", 14 );
    hbd->addSpacing( 10 );

    QString sprites_prefix = ":/trolltech/examples/graphicsview/portedasteroids/sprites/";
/*
    label = new QLabel( tr( "T" ), mainWin );
    label->setFont( smallFont );
    label->setFixedWidth( label->sizeHint().width() );
    label->setPalette( pal );
    hbd->addWidget( label );

    teleportsLCD = new QLCDNumber( 1, mainWin );
    teleportsLCD->setFrameStyle( QFrame::NoFrame );
    teleportsLCD->setSegmentStyle( QLCDNumber::Flat );
    teleportsLCD->setPalette( pal );
    teleportsLCD->setFixedHeight( 20 );
    hbd->addWidget( teleportsLCD );

    hbd->addSpacing( 10 );
*/
    // Set up bottom bar
    //player 1
    label = new QLabel( tr("P1"), mainWin );
    label->setFont( smallFont );
    label->setPalette( pal );
    label->setFixedWidth( label->sizeHint().width() );
    hbd->addWidget( label );

    hbd->addSpacing( 10 );

    QPixmap pm( sprites_prefix + "powerups/brake.png" );
    label = new QLabel( mainWin );
    label->setPixmap( pm );
    label->setFixedWidth( label->sizeHint().width() );
    label->setPalette( pal );
    hbd->addWidget( label );

    player1->brakesLCD = new QLCDNumber( 1, mainWin );
    player1->brakesLCD->setFrameStyle( Q3Frame::NoFrame );
    player1->brakesLCD->setSegmentStyle( QLCDNumber::Flat );
    player1->brakesLCD->setPalette( pal );
    player1->brakesLCD->setFixedHeight( 20 );
    hbd->addWidget( player1->brakesLCD );

    hbd->addSpacing( 10 );

    pm.load( sprites_prefix + "powerups/shield.png" );
    label = new QLabel( mainWin );
    label->setPixmap( pm );
    label->setFixedWidth( label->sizeHint().width() );
    label->setPalette( pal );
    hbd->addWidget( label );

    player1->shieldLCD = new QLCDNumber( 1, mainWin );
    player1->shieldLCD->setFrameStyle( Q3Frame::NoFrame );
    player1->shieldLCD->setSegmentStyle( QLCDNumber::Flat );
    player1->shieldLCD->setPalette( pal );
    player1->shieldLCD->setFixedHeight( 20 );
    hbd->addWidget( player1->shieldLCD );

    hbd->addSpacing( 10 );

    pm.load( sprites_prefix + "powerups/shoot.png" );
    label = new QLabel( mainWin );
    label->setPixmap( pm );
    label->setFixedWidth( label->sizeHint().width() );
    label->setPalette( pal );
    hbd->addWidget( label );

    player1->shootLCD = new QLCDNumber( 1, mainWin );
    player1->shootLCD->setFrameStyle( Q3Frame::NoFrame );
    player1->shootLCD->setSegmentStyle( QLCDNumber::Flat );
    player1->shootLCD->setPalette( pal );
    player1->shootLCD->setFixedHeight( 20 );
    hbd->addWidget( player1->shootLCD );

    hbd->addSpacing( 10 );

    label = new QLabel( tr( "Fuel" ), mainWin );
    label->setFont( smallFont );
    label->setFixedWidth( label->sizeHint().width() + 10 );
    label->setPalette( pal );
    hbd->addWidget( label );

    player1->powerMeter = new KALedMeter( mainWin );
    player1->powerMeter->setFrameStyle( Q3Frame::Box | Q3Frame::Plain );
    player1->powerMeter->setRange( MAX_POWER_LEVEL );
    player1->powerMeter->addColorRange( 10, Qt::darkRed );
    player1->powerMeter->addColorRange( 20, QColor(160, 96, 0) );
    player1->powerMeter->addColorRange( 70, Qt::darkGreen );
    player1->powerMeter->setCount( 40 );
    player1->powerMeter->setPalette( pal );
    player1->powerMeter->setFixedSize( 200, 12 );
    hbd->addWidget( player1->powerMeter );

    // Player 2
    hbd->addStretch( 1 );

    label = new QLabel( tr("P2"), mainWin );
    label->setFont( smallFont );
    label->setPalette( pal );
    label->setFixedWidth( label->sizeHint().width() );
    hbd->addWidget( label );

    hbd->addSpacing( 10 );

    label = new QLabel( mainWin );
    label->setPixmap( pm );
    label->setFixedWidth( label->sizeHint().width() );
    label->setPalette( pal );
    hbd->addWidget( label );

    player2->brakesLCD = new QLCDNumber( 1, mainWin );
    player2->brakesLCD->setFrameStyle( Q3Frame::NoFrame );
    player2->brakesLCD->setSegmentStyle( QLCDNumber::Flat );
    player2->brakesLCD->setPalette( pal );
    player2->brakesLCD->setFixedHeight( 20 );
    hbd->addWidget( player2->brakesLCD );

    hbd->addSpacing( 10 );

    pm.load( sprites_prefix + "powerups/shield.png" );
    label = new QLabel( mainWin );
    label->setPixmap( pm );
    label->setFixedWidth( label->sizeHint().width() );
    label->setPalette( pal );
    hbd->addWidget( label );

    player2->shieldLCD = new QLCDNumber( 1, mainWin );
    player2->shieldLCD->setFrameStyle( Q3Frame::NoFrame );
    player2->shieldLCD->setSegmentStyle( QLCDNumber::Flat );
    player2->shieldLCD->setPalette( pal );
    player2->shieldLCD->setFixedHeight( 20 );
    hbd->addWidget( player2->shieldLCD );

    hbd->addSpacing( 10 );

    pm.load( sprites_prefix + "powerups/shoot.png" );
    label = new QLabel( mainWin );
    label->setPixmap( pm );
    label->setFixedWidth( label->sizeHint().width() );
    label->setPalette( pal );
    hbd->addWidget( label );

    player2->shootLCD = new QLCDNumber( 1, mainWin );
    player2->shootLCD->setFrameStyle( Q3Frame::NoFrame );
    player2->shootLCD->setSegmentStyle( QLCDNumber::Flat );
    player2->shootLCD->setPalette( pal );
    player2->shootLCD->setFixedHeight( 20 );
    hbd->addWidget( player2->shootLCD );

    hbd->addSpacing( 10 );

    label = new QLabel( tr( "Fuel" ), mainWin );
    label->setFont( smallFont );
    label->setFixedWidth( label->sizeHint().width() + 10 );
    label->setPalette( pal );
    hbd->addWidget( label );

    player2->powerMeter = new KALedMeter( mainWin );
    player2->powerMeter->setFrameStyle( Q3Frame::Box | Q3Frame::Plain );
    player2->powerMeter->setRange( MAX_POWER_LEVEL );
    player2->powerMeter->addColorRange( 10, Qt::darkRed );
    player2->powerMeter->addColorRange( 20, QColor(160, 96, 0) );
    player2->powerMeter->addColorRange( 70, Qt::darkGreen );
    player2->powerMeter->setCount( 40 );
    player2->powerMeter->setPalette( pal );
    player2->powerMeter->setFixedSize( 200, 12 );
    hbd->addWidget( player2->powerMeter );

    player1->shipsRemain = 3;
    player2->shipsRemain = 3;
    showHiscores = FALSE;


    // Player 1 keys
    player1->actions.insert( Qt::Key_Up, Thrust );
    player1->actions.insert( Qt::Key_Left, RotateLeft );
    player1->actions.insert( Qt::Key_Right, RotateRight );
    player1->actions.insert( Qt::Key_Space, Shoot );
    player1->actions.insert( Qt::Key_Z, Teleport );
    player1->actions.insert( Qt::Key_X, Brake );
    player1->actions.insert( Qt::Key_S, Shield );
    player1->actions.insert( Qt::Key_P, Pause );
    player1->actions.insert( Qt::Key_L, Launch );
    player1->actions.insert( Qt::Key_N, NewGame );

    // Player 2 Keys
    player2->actions.insert( Qt::Key_8, Thrust );
    player2->actions.insert( Qt::Key_4, RotateLeft );
    player2->actions.insert( Qt::Key_6, RotateRight );
    player2->actions.insert( Qt::Key_Enter, Shoot );
    player2->actions.insert( Qt::Key_5, Teleport );
    player2->actions.insert( Qt::Key_0, Brake );
    player2->actions.insert( Qt::Key_Plus, Shield );
    player2->actions.insert( Qt::Key_division, Pause );
    player2->actions.insert( Qt::Key_7, Launch );
    player2->actions.insert( Qt::Key_M, NewGame );

    Keys.player1_thrust = Qt::Key_Up;
    Keys.player1_left = Qt::Key_Left;
    Keys.player1_right = Qt::Key_Right;
    Keys.player1_shoot = Qt::Key_Space;
    Keys.player1_shield = Qt::Key_S;
    Keys.player1_brake = Qt::Key_X;

    Keys.player2_thrust = Qt::Key_8;
    Keys.player2_left = Qt::Key_4;
    Keys.player2_right = Qt::Key_6;
    Keys.player2_shoot = Qt::Key_Enter;
    Keys.player2_shield = Qt::Key_Plus;
    Keys.player2_brake = Qt::Key_0;

    keySettings = new Settings(&Keys);
    connect(keySettings, SIGNAL( SubmitKeyChange(KeySettings) ), this, SLOT( mapKeys(KeySettings) ));

    view->showText( tr( "N - 1 Player ---- M - 2 Players" ), Qt::yellow );
}

KAstTopLevel::~KAstTopLevel()
{
}

void KAstTopLevel::playSound( const char * )
{
}

void KAstTopLevel::keyPressEvent( QKeyEvent *event )
{
    Action a;

    if(event->key() == Qt::Key_F1)
    {
        keySettings->show();
        return;
    }

    if (event->isAutoRepeat() || !player1->actions.contains(event->key())) {
        if (event->isAutoRepeat() || !player2->actions.contains(event->key())) {
            event->ignore();
            return;
        }

        a = player2->actions[ event->key() ];

        switch ( a )
        {
        case RotateLeft:
            view->rotateLeft(player2, TRUE);
            break;

        case RotateRight:
            view->rotateRight(player2, TRUE);
            break;

        case Thrust:
            view->thrust(player2, TRUE);
            break;

        case Shoot:
            player2->shoot(TRUE);
            break;

        case Shield:
            view->setShield(player2, TRUE);
            break;

        case Teleport:
            view->teleport(player2, TRUE);
            break;

        case Brake:
            view->brake(player2, TRUE);
            break;

        default:
            event->ignore();
            return;
        }

        event->accept();
        return;
    }

    a = player1->actions[ event->key() ];

    switch (a)
    {
    case RotateLeft:
        view->rotateLeft(player1, TRUE);
        break;

    case RotateRight:
        view->rotateRight(player1, TRUE);
        break;

    case Thrust:
        view->thrust(player1, TRUE);
        break;

    case Shoot:
        player1->shoot(TRUE);
        break;

    case Shield:
        view->setShield(player1, TRUE);
        break;

    case Teleport:
        view->teleport(player1, TRUE);
        break;

    case Brake:
        view->brake(player1, TRUE);
        break;

    default:
        event->ignore();
        break;
    }

    event->accept();

}

void KAstTopLevel::keyReleaseEvent( QKeyEvent *event )
{
    Action a;

    if (event->isAutoRepeat() || !player1->actions.contains(event->key())) {
        if (event->isAutoRepeat() || !player2->actions.contains(event->key())) {
            event->ignore();
            return;
        }

        a = player2->actions[event->key()];

        switch (a)
        {
        case RotateLeft:
            view->rotateLeft(player2, FALSE);
            break;

        case RotateRight:
            view->rotateRight(player2, FALSE);
            break;

        case Thrust:
            view->thrust(player2, FALSE);
            break;

        case Shoot:
            player2->shoot(FALSE);
            break;

        case Brake:
            view->brake(player2, FALSE);
            break;

        case Shield:
            view->setShield(player2, FALSE);
            break;

        case Teleport:
            view->teleport(player2, FALSE);
            break;

        case Launch:
            if (player2->waitShip)
            {
                view->newShip(player2);
                player2->waitShip = FALSE;
                view->hideText();
            }
            else
            {
                event->ignore();
                return;
            }
            break;

        case NewGame:
            slotNewGame(true);
            break;
            /*
            case Pause:
                {
                    view->pause( TRUE );
                    QMessageBox::information( this,
                                              tr("KAsteroids is paused"),
                                              tr("Paused") );
                    view->pause( FALSE );
                }
                break;
    */
        default:
            event->ignore();
            return;
        }

        event->accept();
        return;
    }

    a = player1->actions[event->key()];

    switch (a)
    {
    case RotateLeft:
        view->rotateLeft(player1, FALSE);
        break;

    case RotateRight:
        view->rotateRight(player1, FALSE);
        break;

    case Thrust:
        view->thrust(player1, FALSE);
        break;

    case Shoot:
        player1->shoot(FALSE);
        break;

    case Brake:
        view->brake(player1, FALSE);
        break;

    case Shield:
        view->setShield(player1, FALSE);
        break;

    case Teleport:
        view->teleport(player1, FALSE);
        break;

    case Launch:
        if (player1->waitShip)
        {
            view->newShip(player1);
            player1->waitShip = FALSE;
            view->hideText();
        }
        else
        {
            event->ignore();
            return;
        }
        break;

	case NewGame:
        slotNewGame(false);
	    break;
        /*
        case Pause:
            {
                view->pause( TRUE );
                QMessageBox::information( this,
                                          tr("KAsteroids is paused"),
                                          tr("Paused") );
                view->pause( FALSE );
            }
            break; */
    default:
        event->ignore();
        return;
    }

    event->accept();
}

void KAstTopLevel::showEvent( QShowEvent *e )
{
    Q3MainWindow::showEvent( e );
    view->pause( FALSE );
    view->setFocus();
}

void KAstTopLevel::hideEvent( QHideEvent *e )
{
    Q3MainWindow::hideEvent( e );
    view->pause( TRUE );
}

void KAstTopLevel::slotNewGame(bool twoPlayer)
{
    player1->score = 0;
    player2->score = 0;
    player1->shipsRemain = SB_SHIPS;
    if(twoPlayer)
        player2->shipsRemain = SB_SHIPS;
    else
        player2->shipsRemain = 1;
    player1->scoreLCD->display( 0 );
    player2->scoreLCD->display( 0 );
    level = 0;
    levelLCD->display( level+1 );
    player1->shipsLCD->display( player1->shipsRemain-1 );
    player2->shipsLCD->display( player2->shipsRemain-1 );
    view->newGame();
    view->setRockSpeed( levels[0].rockSpeed );
    view->addRocks( levels[0].nrocks );
//    view->showText( tr( "Press L to launch." ), yellow );
    view->newShip(player1);
    player1->waitShip = FALSE;
    player2->waitShip = FALSE;
    view->hideText();
    isPaused = FALSE;
}

void KAstTopLevel::slotShipKilled(Player *p)
{
    p->shipsRemain--;
    p->shipsLCD->display( player1->shipsRemain-1 );

    playSound( "ShipDestroyed" );

    if (p->shipsRemain )
    {
        p->waitShip = TRUE;
        view->showText( tr( "Ship Destroyed. Press L to launch."), Qt::yellow );
    }
    else if(!player1->shipsRemain || !player2->shipsRemain)
    {
        view->showText( tr("Game Over!"), Qt::red );
        view->endGame();
    doStats();
//        highscore->addEntry( score, level, showHiscores );
    }
}

void KAstTopLevel::slotRockHit( int size )
{
    switch ( size )
    {
	case 0:
        player1->score += 10;
         break;

	case 1:
        player1->score += 20;
	    break;

	default:
        player1->score += 40;
      }

    playSound( "RockDestroyed" );

    player1->scoreLCD->display( player1->score );
    player2->scoreLCD->display( player1->score );
}

void KAstTopLevel::slotRocksRemoved()
{
    level++;

    if ( level >= MAX_LEVELS )
    level = MAX_LEVELS - 1;

    view->setRockSpeed( levels[level-1].rockSpeed );
    view->addRocks( levels[level-1].nrocks );

    levelLCD->display( level+1 );
}

void KAstTopLevel::doStats()
{
    QString r( "0.00" );
    if ( player1->shots() )
     r = QString::number( (double)player1->hits() / player1->shots() * 100.0,
                 'g', 2 );

/* multi-line text broken in Qt 3
    QString s = tr( "Game Over\n\nShots fired:\t%1\n  Hit:\t%2\n  Missed:\t%3\nHit ratio:\t%4 %\n\nPress N for a new game" )
      .arg(view->shots()).arg(view->hits())
      .arg(view->shots() - view->hits())
      .arg(r);
*/

    view->showText( "Game Over: N - 1 Player ---- M - 2 Players.", Qt::yellow, FALSE );
}

void KAstTopLevel::slotUpdateVitals()
{
    // Player 1
    player1->brakesLCD->display(view->brakeCount(player1) );
    player1->shieldLCD->display(view->shieldCount(player1) );
    player1->shootLCD->display(view->shootCount(player1) );
    player1->powerMeter->setValue(player1->power() );

    // Player 2
    player2->brakesLCD->display(view->brakeCount(player2));
    player2->shieldLCD->display(view->shieldCount(player2));
    player2->shootLCD->display(view->shootCount(player2));
    player2->powerMeter->setValue(player2->power());
}

void KAstTopLevel::mapKeys( KeySettings newSettings )
{
    player1->actions.remove( Keys.player1_thrust );
    player1->actions.remove( Keys.player1_left );
    player1->actions.remove( Keys.player1_right );
    player1->actions.remove( Keys.player1_shoot );
    // player1->actions.insert( Qt::Key_Z, Teleport );
    player1->actions.remove( Keys.player1_brake );
    player1->actions.remove( Keys.player1_shield );

    player1->actions.insert( newSettings.player1_thrust, Thrust );
    player1->actions.insert( newSettings.player1_left, RotateLeft );
    player1->actions.insert( newSettings.player1_right, RotateRight );
    player1->actions.insert( newSettings.player1_shoot, Shoot );
    // player1->actions.insert( Qt::Key_Z, Teleport );
    player1->actions.insert( newSettings.player1_brake, Brake );
    player1->actions.insert( newSettings.player1_shield, Shield );

    player2->actions.remove( Keys.player2_thrust );
    player2->actions.remove( Keys.player2_left );
    player2->actions.remove( Keys.player2_right );
    player2->actions.remove( Keys.player2_shoot );
    // player2->actions.insert( Qt::Key_Z, Teleport );
    player2->actions.remove( Keys.player2_brake );
    player2->actions.remove( Keys.player2_shield );

    player2->actions.insert( newSettings.player2_thrust, Thrust );
    player2->actions.insert( newSettings.player2_left, RotateLeft );
    player2->actions.insert( newSettings.player2_right, RotateRight );
    player2->actions.insert( newSettings.player2_shoot, Shoot );
    // player1->actions.insert( Qt::Key_Z, Teleport );
    player2->actions.insert( newSettings.player2_brake, Brake );
    player2->actions.insert( newSettings.player2_shield, Shield );

    Keys = newSettings;
}
