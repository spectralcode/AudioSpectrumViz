QT += core gui widgets multimedia printsupport

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

QCUSTOMPLOTDIR = $$shell_path($$PWD/thirdparty/QCustomPlot)
FFTW3DIR = $$shell_path($$PWD/thirdparty/fftw3)

INCLUDEPATH +=  \
	$$QCUSTOMPLOTDIR \
	$$FFTW3DIR

SOURCES += \
	$$QCUSTOMPLOTDIR/qcustomplot.cpp \
	src/audiosettingswidget.cpp \
	src/spectrumcalculator.cpp \
	src/audioinput.cpp \
	src/audiowriter.cpp \
	src/main.cpp \
	src/audiospectrumviz.cpp \
	src/plotwidget.cpp

HEADERS += \
	$$QCUSTOMPLOTDIR/qcustomplot.h \
	src/audiosettingswidget.h \
	src/spectrumcalculator.h \
	src/audioinput.h \
	src/audiospectrumviz.h \
	src/audiowriter.h \
	src/plotwidget.h

FORMS += \
	src/audiosettingswidget.ui \
	src/audiospectrumviz.ui

TRANSLATIONS += \
	languages/audiospectrumviz_en_150.ts

unix{
	LIBS += -lfftw3
}
win32{
	LIBS += -L$$FFTW3DIR -llibfftw3-3
	DEPENDPATH += $$FFTW3DIR
}


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
