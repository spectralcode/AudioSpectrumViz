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

	//write new data to raw audio buffer
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
