#ifndef SPECTRUMCALCULATOR_H
#define SPECTRUMCALCULATOR_H

#include <QObject>
#include <QVector>
#include "fftw3.h"

class SpectrumCalculator : public QObject
{
	Q_OBJECT
public:
	explicit SpectrumCalculator(QObject *parent = nullptr);
	~SpectrumCalculator();

private:
	int inputLength;
	double* inputBuffer;
	fftw_complex* outputBuffer;
	fftw_plan plan;
	QVector<qreal> spectrum;
	QVector<qreal> window;
	bool logarithmEnabled;

	void calculateHanningWindow(int size);


public slots:
	void setSignalLength(int length);
	void calculateSpectrum(QVector<qreal> data);
	void enableLogarithm(bool enable);

signals:
	void spectrumCalculated(QVector<qreal> spectrum);

};

#endif // SPECTRUMCALCULATOR_H
