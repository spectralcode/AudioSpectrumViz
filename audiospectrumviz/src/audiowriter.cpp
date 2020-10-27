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

#include "audiowriter.h"

AudioWriter::AudioWriter()
{
	this->currBufferId = 0;
	this->setBufferLengthInSamples(1024);
}

void AudioWriter::setFormat(QAudioFormat format){
	this->format = format;
	//todo: switch case bit depth check for max and min value for normalization in writeData
}

qint64 AudioWriter::readData(char *data, qint64 maxSize) {
	Q_UNUSED(data)
	Q_UNUSED(maxSize)
	return -1;
}

qint64 AudioWriter::writeData(const char *data, qint64 availableSamples) {
	//move samples in raw audio data for live scrolling effect
	int start = 0;
	if (availableSamples < this->samples) {
		start = this->samples - availableSamples;
		for (int i = 0; i < start; i++){
			this->rawAudioData[i]=this->rawAudioData.at(i + availableSamples);
		}
	}

	//write new data to raw audio buffer //todo: find a way to enable double buffering and test it to see if performance improves
	for (int i = start; i < this->samples; i++, data++){
		this->rawAudioData[i] = static_cast<qreal>((static_cast<uchar>(*data) -128) / static_cast<qreal>(128));
	}

	emit newData(this->rawAudioData);
	return (this->samples - start);
}

void AudioWriter::setBufferLengthInSamples(int samples) {
	this->samples = samples;

	//rezize buffer length if necessary
	if (this->rawAudioData.length() != this->samples) {
		this->rawAudioData.resize(this->samples);
	}
}
