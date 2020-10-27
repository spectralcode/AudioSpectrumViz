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

#ifndef PLOTWIDGET_H
#define PLOTWIDGET_H

#include "qcustomplot.h"

class PlotWidget : public QCustomPlot
{
	Q_OBJECT
public:
	explicit PlotWidget(QWidget *parent = nullptr);
	~PlotWidget();

	QCustomPlot* plot;
	QVector<qreal> sampleNumbers;
	QVector<qreal> sampleValues;


private:
	QCPTextElement* title;
	double xUpper;
	double xLower;
	double yUpper;
	double yLower;

protected:
	void contextMenuEvent(QContextMenuEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;

signals:

public slots:
	virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
	void plotData(QVector<qreal> data);
	void setXaxisValues(QVector<qreal> xAxisValues);
	void setXaxisText(QString text);
	void setYaxisText(QString text);
	void setXaxisRange(double lower, double upper);
	void setYaxisRange(double lower, double upper);
	void setCurveColer(QColor color);
	void setTitleText(QString text);
	bool saveCurveDataToFile(QString fileName);
	void saveToDisk();
};

#endif // PLOTWIDGET_H
