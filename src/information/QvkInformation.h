/* vokoscreenNG - A desktop recorder
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

#ifndef QVKINFORMATION_H
#define QVKINFORMATION_H

#include "ui_formMainWindow.h"
#include "QvkSpezialSlider.h"
#include "QvkVersion.h"
#include "mainwindow.h"

#include <QWidget>
#include <QElapsedTimer>

class QvkInformation: public QWidget
{ 
Q_OBJECT
public:
  QvkInformation(QvkMainWindow *vkMainWindow, Ui_formMainWindow *ui_mainwindow, QvkSpezialSlider *slider_count_down, QvkSpezialSlider *slider_Second_Wait_Before_Recording );
  virtual ~QvkInformation();
  QvkMainWindow *mainWindow;


public slots:

  
private slots: 
  void slot_cleanup();
  void slot_StorageInfo();
  void slot_newVideoFilename( QString filename );
  void slot_displayRecordTime();

  void slot_timeFirstStart();
  void slot_timeContinueStart();

  void slot_summedTimeAfterPause();

  void slot_newVersionAvailable( QString update );

  void slot_Format( QString value );
  void slot_Videocodec( QString value );
  void slot_Audiocodec( QString value );
  void slot_Frames( int value );


signals:
  

protected:  

  
private:
  Ui_formMainWindow *ui;
  QString newVideoFilename = "";
  QTimer *timerStorageInfo;
  QTimer *timerRecord;
  QElapsedTimer *elapsedTime;

  int int_summed = 0;

  QvkSpezialSlider *sliderCountDown;
  QvkSpezialSlider *sliderSecondWaitBeforeRecording;

  QvkVersion version;

  QString fileName = "";


};

#endif
