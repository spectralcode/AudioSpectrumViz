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

#ifndef AUDIOWRITER_H
#define AUDIOWRITER_H

#include <QIODevice>
#include <QVector>
#include <QAudioFormat>


class AudioWriter : public QIODevice
{
	Q_OBJECT
public:
	AudioWriter();
	void setFormat(QAudioFormat format);

private:
	QAudioFormat format;
	QVector<qreal> rawAudioData;
	int samples;
	int currBufferId;

protected:
	qint64 readData(char *data, qint64 maxSize) override;
	qint64 writeData(const char *data, qint64 maxSize) override;

public slots:
	void setBufferLengthInSamples(int samples);

signals:
	void newData(QVector<qreal> data);
};

#endif // AUDIOWRITER_H




