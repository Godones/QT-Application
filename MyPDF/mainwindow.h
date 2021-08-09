#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "formpdf.h"
#include "mulpdfform.h"
#include <QDialog>
#include <QFile>
#include <QFileDialog>
#include <QInputDialog>
#include <QMainWindow>
#include <QMdiSubWindow>
#include <QSlider>
#include <QSpinBox>
#include <QTabWidget>
#include <QTreeWidgetItem>
#include <pdftable.h>
#include <poppler-qt5.h>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

public slots:
    void updateinfo(int index, int totalpages, qreal zoom);

signals:
    void sentinformation_addfile(QStringList newnames); //增加文件时向showpdfform发射

private:
    void openpdf(QString filepath);

    void openpdfMul(QString filepath);

    void showpdfslot(QString filepath);

    void isbelongQt(QString& dir);

    void inputresposityname(QString& resposity);

    void showpdftable();

    void setActionTF();

private slots:
    void on_openfileaction_triggered();

    void on_createlibaction_triggered();

    void on_openoneaction_triggered();

    void on_addfileaction_triggered();

    void on_fitaction_triggered();

    void on_nextpageaction_triggered();

    void on_prepageaction_triggered();

    void Locatedpage_valchanged();

    void on_fitwindowsaction_triggered();

    void on_allpageaction_triggered();

    void on_tabWidget_currentChanged(int index);

    void on_tabWidget_tabCloseRequested(int index);

    void on_treeWidget_itemClicked(QTreeWidgetItem* item, int column);

    void on_actionTest_triggered();

    void on_bigaction_triggered();

    void on_smalleraction_triggered();

private:
    Ui::MainWindow* ui;

    QString Rootpath; //保存仓库的路径

    //    QSpinBox* Locatedpage;

    QLabel* zoomshow;

    QLabel* allpages;
    QLineEdit* inputpage;

    QString getfinaldirname(const QString& fulldirname);
    void showtable(QString);
    void get_xml_Marks(QString);
    void read_xml(QDomNode node, QTreeWidgetItem* parent);
};
#endif // MAINWINDOW_H
