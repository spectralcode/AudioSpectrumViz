#include "audiospectrumviz.h"
#include "ui_audiospectrumviz.h"

AudioSpectrumViz::AudioSpectrumViz(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::AudioSpectrumViz)
{
	qRegisterMetaType<QVector<qreal>>("QVector<qreal>");

	ui->setupUi(this);
	this->initMenu();
	this->audioSettings = this->ui->widget_audiosettings;
	this->rawPlot = this->ui->plotwidget_raw;
	//this->rawPlot->setCurveColer(Qt::black);
	//this->rawPlot->setTitleText(tr("Audio Input"));
	this->rawPlot->setXaxisText(tr("Sample number"));
	this->rawPlot->setYaxisText(tr("Amplitude"));
	this->rawPlot->setYaxisRange(-0.4,0.4);
	this->spectrumPlot = this->ui->plotwidget_spectrum;
	//this->spectrumPlot->setCurveColer(QColor(255, 150, 155));
	//this->spectrumPlot->setTitleText(tr("Spectrum"));
	this->spectrumPlot->setXaxisText(tr("Frequency in Hz"));
	this->spectrumPlot->setYaxisText(tr("Amplitude"));
	this->onLogarithm(false);

	this->input = new AudioInput();

	connect(this->input, &AudioInput::newData, this->rawPlot, &PlotWidget::plotData);
	connect(this->input, &AudioInput::sampleRate, this->audioSettings, &AudioSettingsWidget::displaySampleRateOfCurrentDevice);
	connect(this->audioSettings, &AudioSettingsWidget::inputDeviceChanged, this->input, &AudioInput::setAudioInput);
	connect(this->audioSettings, &AudioSettingsWidget::sampleRateChanged, this, &AudioSpectrumViz::onSampleRateChange);
	connect(this->audioSettings, &AudioSettingsWidget::sampleRateChanged, this->input, &AudioInput::setSampleRate);

	this->calculator = new SpectrumCalculator();
	connect(this->input, &AudioInput::newData, this->calculator, &SpectrumCalculator::calculateSpectrum);
	connect(this->ui->checkBox_logarithm, &QCheckBox::stateChanged, this->calculator, &SpectrumCalculator::enableLogarithm);
	connect(this->ui->checkBox_logarithm, &QCheckBox::stateChanged, this, &AudioSpectrumViz::onLogarithm);
	connect(this->calculator, &SpectrumCalculator::spectrumCalculated, this->spectrumPlot, &PlotWidget::plotData);

	this->onSampleRateChange(this->audioSettings->getCurrentSampleRate());

	this->input->moveToThread(&acquisitionThread);
	connect(&acquisitionThread, &QThread::finished, this->input, &AudioInput::deleteLater);
	acquisitionThread.start();

	this->calculator->moveToThread(&processingThread);
	connect(&processingThread, &QThread::finished, this->calculator, &SpectrumCalculator::deleteLater);
	processingThread.start();
}

AudioSpectrumViz::~AudioSpectrumViz()
{
	acquisitionThread.quit();
	acquisitionThread.wait();

	processingThread.quit();
	processingThread.wait();
	delete ui;
}

void AudioSpectrumViz::updateXaxisValuesRawPlot(int samplesPerLine) {
	QVector<qreal> xValues;
	xValues.resize(samplesPerLine);
	for (int i = 0; i<samplesPerLine; i++) {
		xValues[i] = i;
	}
	this->rawPlot->setXaxisValues(xValues);
}

void AudioSpectrumViz::updateXaxisValuesSpectrumPlot(int samplesPerLine, int sampleRate) {
	QVector<qreal> xValues;
	xValues.resize(samplesPerLine);
	for (int i = 0; i<samplesPerLine; i++) {
		xValues[i] = i*sampleRate/(samplesPerLine*2);
	}
	this->spectrumPlot->setXaxisValues(xValues);
}

void AudioSpectrumViz::initMenu() {
	//file menu
	QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
	QAction *exitAct = fileMenu->addAction(tr("E&xit"), this, &QWidget::close);
	exitAct->setShortcuts(QKeySequence::Quit);
	exitAct->setStatusTip(tr("Close application"));

	//help menu
	QMenu *helpMenu = this->menuBar()->addMenu(tr("&Help"));

	//about dialog
	QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &AudioSpectrumViz::openAboutDialog);
	aboutAct->setStatusTip(tr("About OCTproZ"));
	aboutAct->setIcon(QIcon(":/icons/octproz_info_icon.png"));
}



void AudioSpectrumViz::onSampleRateChange(int sampleRate) {
	disconnect(this->input, &AudioInput::newData, this->rawPlot, &PlotWidget::plotData);
	disconnect(this->input, &AudioInput::newData, this->calculator, &SpectrumCalculator::calculateSpectrum);
	disconnect(this->calculator, &SpectrumCalculator::spectrumCalculated, this->spectrumPlot, &PlotWidget::plotData);


	int samplesPerLine = sampleRate < 16000 ? SAMPLESPERLINE_FOR_LOWSAMPLERATE : SAMPLESPERLINE_FOR_HIGHSAMPLERATE;
	int samplesPerSpectrum = samplesPerLine/2;
	this->updateXaxisValuesRawPlot(samplesPerLine);
	this->updateXaxisValuesSpectrumPlot(samplesPerSpectrum, sampleRate);

	QMetaObject::invokeMethod(this->input, "setBufferLength", Qt::QueuedConnection, Q_ARG(int, samplesPerLine)); //invokeMethod is used because input is not in the gui thread. Alternatively, signal-slot could be used
	QMetaObject::invokeMethod(this->calculator, "setSignalLength", Qt::QueuedConnection, Q_ARG(int, samplesPerLine));
	QApplication::processEvents();

	connect(this->input, &AudioInput::newData, this->rawPlot, &PlotWidget::plotData);
	connect(this->input, &AudioInput::newData, this->calculator, &SpectrumCalculator::calculateSpectrum);
	connect(this->calculator, &SpectrumCalculator::spectrumCalculated, this->spectrumPlot, &PlotWidget::plotData);
}

void AudioSpectrumViz::onLogarithm(bool logarithmEnabled) {
	if(logarithmEnabled){
		this->spectrumPlot->setYaxisRange(LOWER_LOG_AXIS_VALUE, UPPER_LOG_AXIS_VALUE);
	}else{
		this->spectrumPlot->setYaxisRange(LOWER_LIN_AXIS_VALUE, UPPER_LIN_AXIS_VALUE);
	}
}

void AudioSpectrumViz::openAboutDialog() {
	QMessageBox::about(this, tr("About AudioSpectrumViz"),
		tr("<b>AudioSpectrumViz</b> is a simple software for real-time visualization <br>"
		"of waveform and spectrum of an audio signal, such as microphone <br>"
		"input. FFTW3 is used for calculation of the spectrum and QCustomPlot <br>"
		"is used for plotting.<br><br>"
		"<small>AudioSpectrumViz is free software: you can redistribute it and/or modify<br>"
		"it under the terms of the GNU General Public License as published by<br>"
		"the Free Software Foundation, either version 3 of the License, or<br>"
		"(at your option) any later version.<br>"
		"This program is distributed in the hope that it will be useful,<br>"
		"but WITHOUT ANY WARRANTY; without even the implied warranty of<br>"
		"MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the<br>"
		"GNU General Public License for more details.<br>"
		"You should have received a copy of the GNU General Public License<br>"
		"along with this program. If not, see http://www.gnu.org/licenses/.</small><br><br>"
			"Author:     Miroslav Zabic<br>"
			"Contact:    info"
			"@"
			"spectralcode"
			".com<br>"
			"Date:       ")+ VERSION_DATE + tr("<br>"
			"Version:    ") + VERSION);
}

