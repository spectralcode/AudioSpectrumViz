/**
**  This file is part of AudioSpectrumViz.
**  Copyright (C) Miroslav Zabic
**
**  AudioSpectrumViz is free software: you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program. If not, see http://www.gnu.org/licenses/.
**
****
** Author:	Miroslav Zabic
** Contact:	info
**			at
**			spectralcode.com
****
**/

#ifndef AUDIOINPUT_H
#define AUDIOINPUT_H

#include <QObject>
#include "audiowriter.h"
#include <QAudioInput>
#include <QAudioDeviceInfo>
#include <QDebug>

class AudioInput : public QObject
{
	Q_OBJECT
public:
	explicit AudioInput(QObject *parent = nullptr);

private:
	AudioWriter* audioToBufferWriter;
	QAudioInput* audioInputDevice;
	QAudioDeviceInfo inputDeviceInfo;
	QAudioFormat format;

public slots:
	void start();
	void stop();
	void onStateChange(QAudio::State state);
	void setAudioInput(QAudioDeviceInfo device);
	void setSampleRate(int sampleRate);
	void setBufferLength(int samples);

signals:
	void newData(QVector<qreal>);
	void sampleRate(int sampleRate);

};

#endif // AUDIOINPUT_H
