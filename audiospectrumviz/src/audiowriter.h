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




