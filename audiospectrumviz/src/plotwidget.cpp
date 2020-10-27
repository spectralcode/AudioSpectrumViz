#include "plotwidget.h"


PlotWidget::PlotWidget(QWidget *parent) : QCustomPlot(parent){
	this->title = nullptr;
	this->setXaxisRange(-1.0,1.0);
	this->setYaxisRange(-1.0,1.0);
	//set style
//	this->setBackground(QColor(80, 80, 80));
//	//this->axisRect()->setBackground(QColor(55, 55, 55));
//	this->axisRect()->setBackground(QColor(25, 25, 25));
//	this->xAxis->setBasePen(QPen(Qt::white, 1));
//	this->yAxis->setBasePen(QPen(Qt::white, 1));
//	this->xAxis->setTickPen(QPen(Qt::white, 1));
//	this->yAxis->setTickPen(QPen(Qt::white, 1));
//	this->xAxis->setSubTickPen(QPen(Qt::white, 1));
//	this->yAxis->setSubTickPen(QPen(Qt::white, 1));
//	this->xAxis->setTickLabelColor(Qt::white);
//	this->yAxis->setTickLabelColor(Qt::white);
//	this->xAxis->setLabelColor(Qt::white);
//	this->yAxis->setLabelColor(Qt::white);

	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	this->addGraph();
	this->graph(0)->setPen(QPen(Qt::red));

	this->setXaxisText(tr("Sample number"));
	this->setYaxisText(tr("Amplitude"));
	this->legend->setVisible(false);

	this->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

	this->yAxis->setRange(-1.0, 1.0);


	//maximize size of plot area
	//this->axisRect()->setAutoMargins(QCP::msNone);
	this->axisRect()->setMargins(QMargins(0,0,0,0));
}

PlotWidget::~PlotWidget(){
}


void PlotWidget::plotData(QVector<qreal> data){
	this->graph(0)->setData(this->sampleNumbers, data, true);
	this->replot();
	QCoreApplication::processEvents();
}

void PlotWidget::setXaxisValues(QVector<qreal> xAxisValues) {
	this->sampleNumbers.resize(xAxisValues.size());
	this->sampleNumbers = xAxisValues;
	this->setXaxisRange(xAxisValues.first(), xAxisValues.last());
}

void PlotWidget::setXaxisText(QString text) {
	this->xAxis->setLabel(text);
}

void PlotWidget::setYaxisText(QString text) {
	this->yAxis->setLabel(text);
}

void PlotWidget::setXaxisRange(double lower, double upper) {
	this->xLower = lower;
	this->xUpper = upper;
	this->xAxis->setRange(lower, upper);
}

void PlotWidget::setYaxisRange(double lower, double upper) {
	this->yLower = lower;
	this->yUpper = upper;
	this->yAxis->setRange(lower, upper);
}

void PlotWidget::setCurveColer(QColor color) {
	QPen pen = QPen(color);
	this->graph(0)->setPen(pen);;
}

void PlotWidget::setTitleText(QString text) {
	if(this->title == nullptr){
		this->title = new QCPTextElement(this, text, 12);
		this->plotLayout()->insertRow(0);
		this->plotLayout()->addElement(0, 0, title);
	}else{
		this->title->setText(text);
	}
}

void PlotWidget::contextMenuEvent(QContextMenuEvent *event){
	QMenu menu(this);
	QAction savePlotAction(tr("Save Plot as..."), this);
	connect(&savePlotAction, &QAction::triggered, this, &PlotWidget::saveToDisk);
	menu.addAction(&savePlotAction);
	menu.exec(event->globalPos());
}

void PlotWidget::mouseMoveEvent(QMouseEvent *event){
	if(!(event->buttons() & Qt::LeftButton)){
		double x = this->xAxis->pixelToCoord(event->pos().x());
		double y = this->yAxis->pixelToCoord(event->pos().y());
		this->setToolTip(QString("%1 , %2").arg(x).arg(y));
	}else{
		QCustomPlot::mouseMoveEvent(event);
	}
}


void PlotWidget::mouseDoubleClickEvent(QMouseEvent *event) {
	this->setXaxisRange(this->xLower, this->xUpper);
	this->setYaxisRange(this->yLower, this->yUpper);
	this->replot();
	QCustomPlot::mouseDoubleClickEvent(event);
}

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

void PlotWidget::saveToDisk() {
	QString filters("Image (*.png);;Vector graphic (*.pdf);;CSV (*.csv)");
	QString defaultFilter("CSV (*.csv)");
	QString fileName = QFileDialog::getSaveFileName(this, tr("Save Plot"), QDir::currentPath(), filters, &defaultFilter);
	if(fileName == ""){
		//emit error(tr("Save plot to disk canceled."));
		return;
	}
	bool saved = false;
	if(defaultFilter == "Image (*.png)"){
		saved = this->savePng(fileName);
	}else if(defaultFilter == "Vector graphic (*.pdf)"){
		saved = this->savePdf(fileName);
	}else if(defaultFilter == "CSV (*.csv)"){
		saved = this->saveCurveDataToFile(fileName);
	}
	if(saved){
		//emit info(tr("Plot saved to ") + fileName);
	}else{
		//emit error(tr("Could not save plot to disk."));
	}
	//todo: display info and error messages
}


bool PlotWidget::saveCurveDataToFile(QString fileName) {
	bool saved = false;
	QFile file(fileName);
	if (file.open(QFile::WriteOnly|QFile::Truncate)) {
		QTextStream stream(&file);
		stream << this->xAxis->label() << ";" << this->yAxis->label() << "\n";
		for(int i = 0; i < this->sampleNumbers.size(); i++){
			stream << QString::number(this->sampleNumbers.at(i)) << ";" << QString::number(this->graph(0)->data()->at(i)->value) << "\n";
		}
	file.close();
	saved = true;
	}
	return saved;
}
