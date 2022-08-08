/* vokoscreenNG - A desktop recorder
 * Copyright (C) 2017-2019 Volker Kohaupt
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

#ifndef QVKMAGNIFIER_H
#define QVKMAGNIFIER_H

#include <QScreen>
#include <QPixmap>
#include <QDialog>
#include <QLabel>
#include <QApplication>
#include <QTimer>

class QvkMagnifier: public QDialog
{ 
Q_OBJECT
public:
  QvkMagnifier();
  virtual ~QvkMagnifier();
  

public slots:
  void slot_magnifierShow( bool value );
  void slot_magnifier200x200();
  void slot_magnifier400x200();
  void slot_magnifier600x200();


private slots:
  void slot_mytimer();
  

protected:  

  
signals:
  
  
private:
  int NewDistanceXLeft();
  int NewDistanceXRight();
  void setMagnifier();
  QLabel *label;
  int border;
  int distanceX;
  int distanceY;
  int faktor;
  QTimer *timer;
  QRect rectVirtualScreen;
};


#endif
