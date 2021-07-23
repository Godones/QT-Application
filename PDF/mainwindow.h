#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <poppler-qt5.h>
#include<QFile>
#include<QDialog>
#include<QDir>
#include<QFileDialog>
#include "formpdf.h"
#include "showpdfform.h"
#include<QDebug>
#include<QInputDialog>
#include<QLineEdit>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


public:
    void openpdf(QString filepath);

    void showpdfslot(QString filepath);

    void isbelongQt(QString &dir);

    void inputresposityname(QString &resposity);

private slots:
    void on_openfileaction_triggered();

    void on_createlibaction_triggered();

    void on_typeaction_triggered();

    void on_openoneaction_triggered();

private:
    Ui::MainWindow *ui;

};
#endif // MAINWINDOW_H
