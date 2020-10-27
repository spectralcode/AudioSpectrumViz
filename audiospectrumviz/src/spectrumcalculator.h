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
