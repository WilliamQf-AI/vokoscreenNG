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

#include "QvkLimitDiskFreeSpace.h"
#include "global.h"

#include <QDebug>
#include <QTimer>
#include <QStorageInfo>
#include <QMessageBox>

QvkLimitDiskFreeSpace::QvkLimitDiskFreeSpace( Ui_formMainWindow *ui_mainwindow, QvkSpezialSlider *vkSpezialSlider )
{
    ui = ui_mainwindow;
    sliderLimitOfFreeDiskSpace = vkSpezialSlider;

    timerStorageSize = new QTimer(this);
    timerStorageSize->setTimerType( Qt::PreciseTimer );
    timerStorageSize->setInterval( 1000 );
    connect( timerStorageSize, SIGNAL( timeout() ), this, SLOT( slot_storageMessagBoxByRecord() ) );
    connect( ui->pushButtonStart, SIGNAL( clicked(bool) ), timerStorageSize, SLOT( start() ) );
    connect( ui->pushButtonStop,  SIGNAL( clicked(bool) ), timerStorageSize, SLOT( stop() ) );
}


QvkLimitDiskFreeSpace::~QvkLimitDiskFreeSpace()
{
}


bool QvkLimitDiskFreeSpace::isStorageOK()
{
    QStorageInfo storage = QStorageInfo(ui->lineEditVideoPath->text() );
    storage.refresh();

    // Stop a record if disk space smaller 250MB(Default) 999MB(Max)
    if ( storage.bytesAvailable() <= ( sliderLimitOfFreeDiskSpace->value() * 1024 * 1024 ) ) {
        return false;
    }
    return true;
}


bool QvkLimitDiskFreeSpace::isStorageOKMessagBoxByStart()
{
    QStorageInfo storage = QStorageInfo(ui->lineEditVideoPath->text() );
    storage.refresh();

    // Stop a record if disk space smaller 250MB(Default) 999MB(Max)
    if ( storage.bytesAvailable() <= ( sliderLimitOfFreeDiskSpace->value() * 1024 * 1024 ) ) {
        timerStorageSize->stop();

        QPixmap pixmap( ":/pictures/status/warning.png" );
        pixmap = pixmap.scaled( 64, 64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );

        QMessageBox *messageBox = new QMessageBox();
        messageBox->setWindowIcon( QIcon( ":/pictures/logo/logo.png" ) );
        messageBox->setWindowTitle( tr( "Warning" ) + " " + global::name + " " + global::version );
        messageBox->setText( tr( "The recording is not started, not enough free disk space." ) );
        messageBox->setInformativeText( tr( "The free disk space limit is reached." )
                                        + "\n"
                                        + "\n"
                                        + tr( "Limit of free disk space" ) + " " + QString::number( sliderLimitOfFreeDiskSpace->value() ) + " " + tr( "Megabyte" )
                                      );
        messageBox->setStandardButtons( QMessageBox::Ok );
        messageBox->setIconPixmap( pixmap );
        messageBox->show();

        return false;
    }

    return true;
}


void QvkLimitDiskFreeSpace::slot_storageMessagBoxByRecord()
{
    QStorageInfo storage = QStorageInfo(ui->lineEditVideoPath->text() );
    storage.refresh();

    // Stop a record if disk space smaller 250MB(Default) 999MB(Max)
    if ( storage.bytesAvailable() <= ( sliderLimitOfFreeDiskSpace->value() * 1024 * 1024 ) ) {
        timerStorageSize->stop();
        ui->pushButtonStop->click();

        QPixmap pixmap( ":/pictures/status/warning.png" );
        pixmap = pixmap.scaled( 64, 64, Qt::IgnoreAspectRatio, Qt::SmoothTransformation );

        QMessageBox *messageBox = new QMessageBox();
        messageBox->setWindowIcon( QIcon( ":/pictures/logo/logo.png" ) );
        messageBox->setWindowTitle( tr( "Warning" ) + " " + global::name + " " + global::version );
        messageBox->setText( tr( "The recording was stopped, not enough free disk space." ) );
        messageBox->setInformativeText( tr( "The free disk space limit is reached." )
                                        + "\n"
                                        + "\n"
                                        + tr( "Limit of free disk space" ) + " " + QString::number( sliderLimitOfFreeDiskSpace->value() ) + " " + tr( "Megabyte" )
                                      );
        messageBox->setStandardButtons( QMessageBox::Ok );
        messageBox->setIconPixmap( pixmap );
        messageBox->show();
    }
}
