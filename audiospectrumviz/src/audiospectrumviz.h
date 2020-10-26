#ifndef AUDIOSPECTRUMVIZ_H
#define AUDIOSPECTRUMVIZ_H

#define VERSION_DATE "October 22, 2020"
#define VERSION "0.1"

#define SAMPLESPERLINE_FOR_LOWSAMPLERATE 1024
#define SAMPLESPERLINE_FOR_HIGHSAMPLERATE 8192

#define LOWER_LOG_AXIS_VALUE -80
#define UPPER_LOG_AXIS_VALUE 0
#define LOWER_LIN_AXIS_VALUE -0.001
#define UPPER_LIN_AXIS_VALUE 0.02


#include <QMainWindow>
#include <QMetaObject>
#include "audioinput.h"
#include "audiosettingswidget.h"
#include "plotwidget.h"
#include "spectrumcalculator.h"



QT_BEGIN_NAMESPACE
namespace Ui { class AudioSpectrumViz; }
QT_END_NAMESPACE

class AudioSpectrumViz : public QMainWindow
{
	Q_OBJECT
	QThread acquisitionThread;
	QThread processingThread;

public:
	AudioSpectrumViz(QWidget *parent = nullptr);
	~AudioSpectrumViz();

private:
	Ui::AudioSpectrumViz *ui;
	AudioInput* input;
	SpectrumCalculator* calculator;
	AudioSettingsWidget* audioSettings;
	PlotWidget* rawPlot;
	PlotWidget* spectrumPlot;

	void updateXaxisValuesRawPlot(int samplesPerLine);
	void updateXaxisValuesSpectrumPlot(int samplesPerLine, int sampleRate);
	void initMenu();

public slots:
	void onSampleRateChange(int sampleRate);
	void onLogarithm(bool logarithmEnabled);
	void openAboutDialog();
};
#endif // AUDIOSPECTRUMVIZ_H
