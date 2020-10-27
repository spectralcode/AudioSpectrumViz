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
