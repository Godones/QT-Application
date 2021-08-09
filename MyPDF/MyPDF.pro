QT       += core gui

QT +=xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    dialogpdf.cpp \
    formpdf.cpp \
    main.cpp \
    mainwindow.cpp \
    mulpage.cpp \
    mulpdfform.cpp \
    pagerender.cpp \
    pdftable.cpp

HEADERS += \
    dialogpdf.h \
    formpdf.h \
    mainwindow.h \
    mulpage.h \
    mulpdfform.h \
    pagerender.h \
    pdftable.h

FORMS += \
    dialogpdf.ui \
    formpdf.ui \
    mainwindow.ui \
    mulpdfform.ui \
    pdftable.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
INCLUDEPATH += $$PWD/poppler
win32: LIBS += -L$$PWD/poppler -llibpoppler
win32: LIBS += -L$$PWD/poppler -llibpoppler-qt5

RESOURCES += \
    icons.qrc
RC_ICONS += pdf.ico

