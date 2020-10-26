#include "audioinput.h"


AudioInput::AudioInput(QObject *parent) : QObject(parent)
{
	this->audioToBufferWriter = new AudioWriter();
	connect(this->audioToBufferWriter, &AudioWriter::newData, this, &AudioInput::newData);

	this->inputDeviceInfo = QAudioDeviceInfo::defaultInputDevice();

	this->format.setSampleRate(8000);
	this->format.setChannelCount(1);
	this->format.setSampleSize(8);
	this->format.setCodec("audio/pcm");
	this->format.setByteOrder(QAudioFormat::LittleEndian);
	this->format.setSampleType(QAudioFormat::UnSignedInt);

	this->start();
}

void AudioInput::start() {
	if (!inputDeviceInfo.isFormatSupported(this->format)) {
		this->format = inputDeviceInfo.nearestFormat(this->format);
	}
	this->audioToBufferWriter->setFormat(this->format);

	//create audio input device and set audio format
	this->audioInputDevice = new QAudioInput(this->inputDeviceInfo, this->format, this);
	connect(this->audioInputDevice, &QAudioInput::stateChanged, this, &AudioInput::onStateChange);

	//start acquisition
	this->audioToBufferWriter->open(QIODevice::WriteOnly);
	this->audioInputDevice->start(audioToBufferWriter);

	QAudioFormat usedFormat = this->audioInputDevice->format();
	emit sampleRate(usedFormat.sampleRate());
//	emit info("used sample rate: " + QString::number(usedFormat.sampleRate()));
//	emit info("used channel count: " + QString::number(usedFormat.channelCount()));
//	emit info("used sample size: " + QString::number(usedFormat.sampleSize()));
//	emit info("used codec: " + usedFormat.codec());
}

void AudioInput::stop() {
	this->audioInputDevice->stop();
	delete this->audioInputDevice;
	this->audioInputDevice = nullptr;
}

void AudioInput::onStateChange(QAudio::State state) {
	//todo: rethink if this slot is really necessary
	switch (state) {
		 case QAudio::StoppedState:
			if(this->audioInputDevice->error() != QAudio::NoError) {
				qDebug() << "Audio input device error! ";
			} else {
				qDebug() << "Audio input device stopped.";
			}
			break;
		 case QAudio::ActiveState:
			qDebug() << "Audio input device active";
			break;
		case QAudio::SuspendedState:
			qDebug() <<"Audio input device suspended";
			break;
		case QAudio::IdleState:
			qDebug() <<"Audio input device idle";
			break;
		case QAudio::InterruptedState:
			qDebug() <<"Audio input device interrupted";
			break;
		 default:
			qDebug() << "Audio input device is in unknown state";
			break;
	}
}

void AudioInput::setAudioInput(QAudioDeviceInfo device) {
	if (device.deviceName() != this->inputDeviceInfo.deviceName()) {
		//todo: check if device active before stopping
		this->stop();
		this->inputDeviceInfo = device;
		this->start();
	}
}

void AudioInput::setSampleRate(int sampleRate) {
	if (sampleRate != this->format.sampleRate()){
		this->stop();
		this->format.setSampleRate(sampleRate);
		this->start();
	}
}

void AudioInput::setBufferLength(int samples) {
	this->audioToBufferWriter->setBufferLengthInSamples(samples);
}
