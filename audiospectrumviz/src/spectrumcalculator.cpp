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

#include "spectrumcalculator.h"
#include <QtMath>

#define REAL 0
#define IMAG 1

SpectrumCalculator::SpectrumCalculator(QObject *parent) : QObject(parent)
{
	this->inputBuffer = nullptr;
	this->logarithmEnabled = false;
	this->setSignalLength(1024);
}

SpectrumCalculator::~SpectrumCalculator() {
	if(this->inputBuffer != nullptr){
		fftw_free(this->inputBuffer);
		this->inputBuffer = nullptr;

		fftw_free(this->outputBuffer);
		this->outputBuffer = nullptr;

		fftw_destroy_plan(this->plan);
		fftw_cleanup();
	}
}

void SpectrumCalculator::setSignalLength(int length) {
	//set input and output lenght. input length is the number of samples within the raw audio data. output length is the number of real samples after fft. note: output contains real AND imaginary part
	this->inputLength = length;
	int outputLength = this->inputLength/2;

	if(this->inputBuffer != nullptr){
		fftw_destroy_plan(this->plan);
		fftw_cleanup();

		fftw_free(this->inputBuffer);
		this->inputBuffer = nullptr;

		fftw_free(this->outputBuffer);
		this->outputBuffer = nullptr;
	}

	this->inputBuffer = static_cast<double*>(fftw_malloc(this->inputLength * sizeof(double)));
	this->outputBuffer = (fftw_complex*)(fftw_malloc((outputLength+1) * sizeof(fftw_complex)));
	this->plan = fftw_plan_dft_r2c_1d(this->inputLength, this->inputBuffer, this->outputBuffer, FFTW_ESTIMATE);
	this->spectrum.resize(outputLength);
	this->calculateHanningWindow(this->inputLength);
}

void SpectrumCalculator::calculateSpectrum(QVector<qreal> data) {
	memcpy(this->inputBuffer, data.data(), data.size()*sizeof (double));
	for(int i = 0; i < this->inputLength; i++){
		this->inputBuffer[i] = this->inputBuffer[i]*this->window[i];
	}
	fftw_execute(this->plan);

	int samples = this->spectrum.length();
	for(int i = 0; i < samples; i++){
		double realVal = this->outputBuffer[i][REAL];
		double imagVal = this->outputBuffer[i][IMAG];

		if(logarithmEnabled){
			this->spectrum[i] = 10*log10(((realVal*realVal+imagVal*imagVal))/(static_cast<double>(this->inputLength))); //Note: log(sqrt(x*x+y*y)) == 0.5*log(x*x+y*y) --> the result here is 20*log(sqrt(x*x+y*y)).
		}else{
			this->spectrum[i] = qSqrt(realVal*realVal+imagVal*imagVal)/(static_cast<double>(this->inputLength));
		}
	}
	emit spectrumCalculated(this->spectrum);
}

void SpectrumCalculator::enableLogarithm(bool enable) {
	this->logarithmEnabled = enable;
}


void SpectrumCalculator::calculateHanningWindow(int size) {
	this->window.resize(size);
	int width = size;
	int center = static_cast<int>(width/2);
	int minPos = center - width/2;
	int maxPos = minPos + width;
	if (maxPos < minPos) {
		int tmp = minPos;
		minPos = maxPos;
		maxPos = tmp;
	}
	for (int i = 0; i<width; i++) {
		int xi = i - minPos;
		qreal xiNorm = (static_cast<qreal>(xi) / (static_cast<qreal>(width) - 1.0));
		if (xiNorm > 0.999 || xiNorm < 0.0001) {
			this->window[i] = 0.0;
		}
		else {
			this->window[i] = (0.5) * (1 - cos(2.0 * M_PI * (xiNorm)));
		}
	}
}
