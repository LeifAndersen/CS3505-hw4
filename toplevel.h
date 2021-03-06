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
/*
 * Modifications from Leif Andersen, Robert Christensen.
 *
 * (c) (on changes) TEAM: KatastrofieMarch 2011
 */
#ifndef __KAST_TOPLEVEL_H__
#define __KAST_TOPLEVEL_H__

#include <q3mainwindow.h>
#include <q3dict.h>
#include <qmap.h>
//Added by qt3to4:
#include <QShowEvent>
#include <QHideEvent>
#include <QKeyEvent>

#include "view.h"
#include "player.h"
#include "settings.h"

class KALedMeter;
QT_BEGIN_NAMESPACE
class QLCDNumber;
QT_END_NAMESPACE

class KAstTopLevel : public Q3MainWindow
{
    Q_OBJECT
public:
    /**
      * creates the new toplevel (such as window)
      */
    KAstTopLevel( QWidget *parent=0, const char *name=0 );

    /**
      * Destructs the game
      */
    virtual ~KAstTopLevel();

private:
    // Currently does nothing
    void playSound( const char *snd );
    void readSoundMapping();
    void doStats();

protected:
    /**
      * Ship show event
      */
    virtual void showEvent( QShowEvent * );

    /**
      * Ship hide event
      */
    virtual void hideEvent( QHideEvent * );

    /**
      * Key press event
      */
    virtual void keyPressEvent( QKeyEvent *event );

    /**
      * Key release event
      */
    virtual void keyReleaseEvent( QKeyEvent *event );

private slots:
    /**
      * Called when a new game is started
      */
    void slotNewGame(bool twoPlayers);

    /**
      * Called when the ship is killed
      */
    void slotShipKilled(Player *p);

    /**
      * Called when a rock is hit
      */
    void slotRockHit(Player *p, int size);

    /**
      * Called when rock is removed
      */
    void slotRocksRemoved();

    /**
      * Called to update vitals
      */
    void slotUpdateVitals();

    /**
      * Map keybindings
      */
    void mapKeys( KeySettings );

private:
    // Window stuff
    KAsteroidsView *view;
    Settings *keySettings;
    KeySettings Keys;
    QLCDNumber *levelLCD;

    // Sound, not used
    bool   sound;
    Q3Dict<QString> soundDict;

    // waiting for user to press Enter to launch a ship
    bool isPaused;

    // the players for the game
    Player *player1;
    Player *player2;

    // level
    int level;

    // highscores, not used.
    bool showHiscores;
};

#endif

