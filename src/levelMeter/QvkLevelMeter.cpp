// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include "QvkLevelMeter.h"

#include <QAudioDevice>
#include <QAudioSource>
#include <QDebug>

QvkQIODevice::QvkQIODevice( const QAudioFormat &format ) : m_format(format)
{
}

void QvkQIODevice::start()
{
    open( QIODevice::WriteOnly );
}

void QvkQIODevice::stop()
{
    close();
}

qint64 QvkQIODevice::readData( char *, qint64 )
{
    return 0;
}

qreal QvkQIODevice::calculateLevel( const char *data, qint64 len ) const
{
    const int channelBytes = m_format.bytesPerSample();
    const int sampleBytes = m_format.bytesPerFrame();
    const int numSamples = len / sampleBytes;

    float maxValue = 0;
    auto *ptr = reinterpret_cast<const unsigned char *>(data);

    for (int i = 0; i < numSamples; ++i) {
        for (int j = 0; j < m_format.channelCount(); ++j) {
            float value = m_format.normalizedSampleValue(ptr);
            maxValue = qMax(value, maxValue);
            ptr += channelBytes;
        }
    }
    return maxValue;
}

qint64 QvkQIODevice::writeData(const char *data, qint64 len)
{
    qreal m_level = calculateLevel( data, len );
    emit signal_levelChanged( m_level );
//    qDebug() << m_level << myDevice;
    return len;
}

//----------------------------------------------------------

InputStart::InputStart( QAudioDevice device )
{
    QAudioFormat format;
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    vkQIODevice = new QvkQIODevice( format );
    vkQIODevice->myDevice = device.id();

    connect( vkQIODevice, SIGNAL( signal_levelChanged(qreal) ), this, SLOT( slot_levelChanged(qreal) ) );
    audioSource = new QAudioSource( device, format );
}

void InputStart::slot_start()
{
    vkQIODevice->start();
    audioSource->start(vkQIODevice);
}

void InputStart::slot_stop()
{
    disconnect( vkQIODevice, SIGNAL( signal_levelChanged(qreal) ), nullptr, nullptr );
    audioSource->stop();
    vkQIODevice->stop();
}

void InputStart::slot_levelChanged( qreal level )
{
    emit signal_level( level * 10000 );
}
