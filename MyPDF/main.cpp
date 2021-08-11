#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char* argv[])
{

    QTextCodec* code = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(code);
    QApplication a(argc, argv);
    qRegisterMetaType<QVector<QPixmap>>("QVector<QPixmap>");
    qRegisterMetaType<QVector<QString>>("QVector<QString>");
    MainWindow w;

    w.show();
    return a.exec();
}
