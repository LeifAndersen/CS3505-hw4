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

#ifndef __AST_VIEW_H__
#define __AST_VIEW_H__

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
#include "sprites.h"

#include "player.h"
#define MAX_POWER_LEVEL          1000


class KAsteroidsView : public QWidget
{
    Q_OBJECT
public:
    KAsteroidsView(Player *player1, Player *player2, QWidget *parent = 0, const char *name = 0);
    virtual ~KAsteroidsView();

    int refreshRate;

    void reset();
    void setRockSpeed( double rs ) { rockSpeed = rs; }
    void addRocks( int num );
    void newGame();
    void endGame();

    void showText( const QString &text, const QColor &color, bool scroll=TRUE );
    void hideText();

    void pause(bool p);

signals:
    void shipKilled(Player *);
    void rockHit( int size );
    void rocksRemoved();
    void updateVitals();


protected:
    bool readSprites();
    void wrapSprite( QGraphicsItem * );
    void rockHit( AnimatedPixmapItem * );
    void addExhaust( double x, double y, double dx, double dy, int count );
    void processMissiles(Player *p);
    void processShip(Player *p);
    void processPowerups(Player *p);
    void processShield();
    double randDouble();
    int randInt( int range );

    virtual void resizeEvent( QResizeEvent *event );
    virtual void timerEvent(QTimerEvent *);

    void showEvent( QShowEvent * );

private:
    QGraphicsScene field;
    QGraphicsView view;
    QMap<int, QList<QPixmap> > animation;
    Q3PtrList<AnimatedPixmapItem> rocks;
    Q3PtrList<KBit> bits;
    Q3PtrList<KExhaust> exhaust;
    Q3PtrList<KPowerup> powerups;
    QGraphicsTextItem *textSprite;

    int  textDy;
    int  mFrameNum;
    bool mPaused;
    int  mTimerId;

    double rockSpeed;
    double powerupSpeed;

    bool can_destroy_powerups;
    bool initialized;

    Player *player1;
    Player *player2;
};

#endif
