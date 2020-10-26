#ifndef AUDIOSETTINGSWIDGET_H
#define AUDIOSETTINGSWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QAudioDeviceInfo>

namespace Ui {
class AudioSettingsWidget;
}

class AudioSettingsWidget : public QWidget
{
	Q_OBJECT

public:
	explicit AudioSettingsWidget(QWidget *parent = nullptr);
	~AudioSettingsWidget();
	int getCurrentSampleRate(){return this->sampleRateComboBox->itemData(this->sampleRateComboBox->currentIndex()).value<int>();}

private:
	Ui::AudioSettingsWidget *ui;
	QComboBox* inputDeviceComboBox;
	QComboBox* sampleRateComboBox;
	QAudioDeviceInfo inputDevice;

	void init();
	void updateSampleRates();

public slots:
	void onInputDeviceChange(int id);
	void onSampleRateChange(int id);
	void displaySampleRateOfCurrentDevice(int sampleRate);

signals:
	void inputDeviceChanged(QAudioDeviceInfo device);
	void sampleRateChanged(int sampleRate);
};

#endif // AUDIOSETTINGSWIDGET_H
