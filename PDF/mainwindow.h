#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <poppler-qt5.h>
#include<QFile>
#include<QDialog>
#include<QDir>
#include<QFileDialog>
#include "formpdf.h"
#include<QDebug>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_openfileaction_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H