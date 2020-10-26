#include "audiospectrumviz.h"

#include <QApplication>

void style() {
	qApp->setStyleSheet(
		//reduce separator width
		"QMainWindow::separator {width: 1px} "
		//adjust menu style
		"QMenu::separator {height: 1px; margin-left: 6px; margin-right: 6px; background: rgba(155, 155, 155, 255);}"
	);

	//set style and color palette
	qApp->setStyle(QStyleFactory::create("Fusion"));
	QPalette dark;
	dark.setColor(QPalette::Text, QColor(255, 255, 255));
	dark.setColor(QPalette::WindowText, QColor(255, 255, 255));
	dark.setColor(QPalette::Window, QColor(50, 50, 50));
	dark.setColor(QPalette::Button, QColor(50, 50, 50));
	dark.setColor(QPalette::Base, QColor(25, 25, 25));
	dark.setColor(QPalette::AlternateBase, QColor(50, 50, 50));
	dark.setColor(QPalette::ToolTipBase, QColor(200, 200, 200));
	dark.setColor(QPalette::ToolTipText, QColor(50, 50, 50));
	dark.setColor(QPalette::ButtonText, QColor(255, 255, 255));
	dark.setColor(QPalette::BrightText, QColor(255, 50, 50));
	dark.setColor(QPalette::Link, QColor(40, 130, 220));
	dark.setColor(QPalette::Disabled, QPalette::Text, QColor(99, 99, 99));
	dark.setColor(QPalette::Disabled, QPalette::WindowText, QColor(99, 99, 99));
	dark.setColor(QPalette::Disabled, QPalette::ButtonText, QColor(99, 99, 99));
	dark.setColor(QPalette::Disabled, QPalette::Highlight, QColor(80, 80, 80));
	dark.setColor(QPalette::Disabled, QPalette::HighlightedText, QColor(99, 99, 99));
	qApp->setPalette(dark);

	//adjust font size
	QFont font;
	font.setFamily(font.defaultFamily());
	font.setPointSize(8);
	qApp->setFont(font);
}

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication a(argc, argv);
	AudioSpectrumViz w;
	w.show();
	return a.exec();
}
