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

#include "audiosettingswidget.h"
#include "ui_audiosettingswidget.h"

AudioSettingsWidget::AudioSettingsWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::AudioSettingsWidget)
{
	ui->setupUi(this);
	this->inputDeviceComboBox = this->ui->comboBox_input;
	this->sampleRateComboBox = this->ui->comboBox_sampleRate;
	this->init();

	connect(this->inputDeviceComboBox, QOverload<int>::of(&QComboBox::activated), this, &AudioSettingsWidget::onInputDeviceChange);
	connect(this->sampleRateComboBox, QOverload<int>::of(&QComboBox::activated), this, &AudioSettingsWidget::onSampleRateChange);
}

AudioSettingsWidget::~AudioSettingsWidget()
{
	delete ui;
}

void AudioSettingsWidget::init() {
	//get available input devices and populate combo box
	QAudioDeviceInfo device;
	QList<QAudioDeviceInfo> availableInputDevices = device.availableDevices(QAudio::AudioInput);
	foreach (device, availableInputDevices){
		this->inputDeviceComboBox->addItem(device.deviceName(),QVariant::fromValue(device));
	}

	//set default input device
	if (!availableInputDevices.empty()){
		this->onInputDeviceChange(0);
	}
}

void AudioSettingsWidget::updateSampleRates() {
	this->sampleRateComboBox->clear();
	int sampleRate;
	foreach (sampleRate, this->inputDevice.supportedSampleRates()){
		this->sampleRateComboBox->addItem(QString::number(sampleRate),QVariant::fromValue(sampleRate));
	}
}

void AudioSettingsWidget::onInputDeviceChange(int id) {
	if(this->inputDeviceComboBox->currentIndex() != id){
		this->inputDeviceComboBox->setCurrentIndex(id);
	}
	this->inputDevice = this->inputDeviceComboBox->itemData(id).value<QAudioDeviceInfo>();
	emit inputDeviceChanged(this->inputDevice);
	this->updateSampleRates();
}

void AudioSettingsWidget::onSampleRateChange(int id) {
	emit sampleRateChanged(this->sampleRateComboBox->itemData(id).value<int>());
}

void AudioSettingsWidget::displaySampleRateOfCurrentDevice(int sampleRate) {
	this->sampleRateComboBox->setCurrentIndex(this->sampleRateComboBox->findData(QVariant::fromValue(sampleRate)));
}
