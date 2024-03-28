﻿/* vokoscreenNG
 * Copyright (C) 2017-2022 Volker Kohaupt
 *
 * Author:
 *      Volker Kohaupt <vkohaupt@volkoh.de>
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * --End_License--
 */

#include "QvkAnimateWindow.h"
#include "global.h"

#include <QPainter>
#include <QDebug>
#include <QTimer>
#include <QBitmap>

QvkAnimateWindow::QvkAnimateWindow( QWidget *parent,
                                    qreal x,
                                    qreal y,
                                    int showTime,
                                    QString button,
                                    qreal valueDiameter,
                                    qreal valueOpacity,
                                    QColor valueColor )
{
    setParent( parent );
    setAttribute( Qt::WA_DeleteOnClose, true ) ;
    setAttribute( Qt::WA_TranslucentBackground, true );

    resize( valueDiameter + 10, valueDiameter + 10 );
    move( x  - ( width() / 2 ), y - ( height() / 2 ) );

    mouseButton = button;
    diameter = valueDiameter;
    opacity = valueOpacity / 100;
    color = valueColor;

    QTimer::singleShot( showTime, Qt::PreciseTimer, this, SLOT( close() ) );
    global::showclickCounter++;
    show();
}


QvkAnimateWindow::~QvkAnimateWindow()
{
}


void QvkAnimateWindow::closeEvent( QCloseEvent *event )
{
    Q_UNUSED(event);
    global::showclickCounter--;
}


void QvkAnimateWindow::paintEvent( QPaintEvent *event )
{
    Q_UNUSED(event);

    QPixmap pixmap( height(), width() );
    pixmap.fill( Qt::transparent );

    QPainter painterPixmap;
    painterPixmap.begin( &pixmap );
    painterPixmap.setRenderHint( QPainter::Antialiasing, true );
    painterPixmap.setRenderHint( QPainter::SmoothPixmapTransform, true );

    qreal penWith = 5.0;
    QPen pen;
    pen.setWidthF( penWith );
    pen.setColor( color );
    pen.setStyle( Qt::SolidLine );
    painterPixmap.setPen( pen );
    painterPixmap.setBrush( Qt::NoBrush );
    painterPixmap.setOpacity( opacity );
    painterPixmap.drawEllipse( width()/2-diameter/2, height()/2-diameter/2, diameter, diameter );

    // Paint pressed Button
    pen.setStyle( Qt::SolidLine );
    pen.setWidthF( 3.0 );
    painterPixmap.setPen( pen );
    painterPixmap.setOpacity( opacity );
    if ( mouseButton == "LeftButton" ) {
        QRectF rectF( width()/2 - diameter/2 + 1.5*penWith, height()/2 - diameter/2 + 1.5*penWith, diameter-3*penWith, diameter-3*penWith );
        qreal startAngle = 90 * 16;
        qreal spanAngle = 180 * 16;
        painterPixmap.drawArc( rectF, startAngle, spanAngle );
    }

    if ( mouseButton == "RightButton" ) {
        QRectF rectF( width()/2 - diameter/2 + 1.5*penWith, height()/2 - diameter/2 + 1.5*penWith, diameter-3*penWith, diameter-3*penWith );
        qreal startAngle = -90 * 16;
        qreal spanAngle = 180 * 16;
        painterPixmap.drawArc( rectF, startAngle, spanAngle );
    }

    if ( mouseButton == "MiddleButton" ) {
        QLineF line( width()/2, height()/2 - diameter/2 + 1.5*penWith , width()/2, height()/2 + diameter/2 - 1.5*penWith );
        painterPixmap.drawLine( line );
    }

    painterPixmap.end();

    QPainter painter;
    painter.begin( this );
    painter.setRenderHint( QPainter::Antialiasing, true );
    painter.setRenderHint( QPainter::SmoothPixmapTransform, true );
    painter.drawPixmap( QPointF( 0, 0 ), pixmap );
    painter.end();

    setMask( pixmap.mask() );
}
