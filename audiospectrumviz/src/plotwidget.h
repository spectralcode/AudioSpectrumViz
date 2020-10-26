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
