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
