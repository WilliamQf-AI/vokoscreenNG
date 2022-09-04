/* vokoscreenNG
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

#ifndef QVKANIMATEWINDOW_H
#define QVKANIMATEWINDOW_H

#include <QWidget>

class QvkAnimateWindow: public QWidget

{
    Q_OBJECT
public:
    QvkAnimateWindow( QWidget *parent,
                                        qreal x,
                                        qreal y,
                                        int showTime,
                                        QString button,
                                        qreal valueDiameter,
                                        qreal valueOpacity,
                                        QColor valueColor );

    virtual ~QvkAnimateWindow();

    
public slots:


private:
    qreal diameter;
    qreal opacity;
    QColor color;
    QString mouseButton;


private slots:


protected:
    void paintEvent( QPaintEvent *event );
    void closeEvent( QCloseEvent *event );


signals:

};

#endif // QVKANIMATEWINDOW_H
