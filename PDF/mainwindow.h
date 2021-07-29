#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <poppler-qt5.h>
#include<QFile>
#include<QDialog>
#include<QFileDialog>
#include "formpdf.h"
#include "showpdfform.h"
#include<QInputDialog>
#include <QSpinBox>
#include<QMdiSubWindow>
#include<QSlider>
#include<pdftable.h>
#include"allpageshow.h"
#include"mygraphics.h"

#include<QTabWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void sentinformation_addfile(QStringList newnames);//增加文件时向showpdfform发射

private:
    void openpdf(QString filepath);

    void showpdfslot(QString filepath);

    void isbelongQt(QString &dir);

    void inputresposityname(QString &resposity);

    void showpdftable();

    void setActionTF(bool arg);



private slots:
    void on_openfileaction_triggered();

    void on_createlibaction_triggered();


    void on_openoneaction_triggered();

    void on_addfileaction_triggered();

    void on_fitaction_triggered();

    void on_nextpageaction_triggered();

    void on_prepageaction_triggered();

    void Locatedpage_valchanged(int num);

    void resetScale(int);

    void on_fitwindowsaction_triggered();

    void on_mdiArea_subWindowActivated(QMdiSubWindow *arg1);

    void on_allpageaction_triggered();

private:
    Ui::MainWindow *ui;

    QString Rootpath;//保存仓库的路径

    QSpinBox *Locatedpage;
    QSlider *Scaling;//缩放因子

    QString getfinaldirname(const QString &fulldirname);
    void showtable();
};
#endif // MAINWINDOW_H
