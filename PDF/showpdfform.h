#ifndef SHOWPDFFORM_H
#define SHOWPDFFORM_H

#include <QWidget>
#include<QImage>
#include<QDir>
#include "poppler-qt5.h"
#include <QDebug>
#include<QMimeDatabase>
#include "QGuiApplication"
#include <QScreen>
#include<QTableWidget>
#include <QLabel>
#include<QMessageBox>


namespace Ui {
class showpdfForm;
}

class showpdfForm : public QWidget
{
    Q_OBJECT

public:
    explicit showpdfForm(QWidget *parent = nullptr);
    ~showpdfForm();

    void init(QString &path);
    void createtable(QVector<QImage>&ico,QVector<QString>&names);
    void resizeEvent(QResizeEvent *event) override;

    void connectSlot();

signals:
    void sentfilepath(QString pdfpath);


private slots:

    void on_tableWidget_cellDoubleClicked(int row, int column);

private:
    QVector<QImage>ico;//存储所有能打开的pdf图标
    QVector<QString>name;//存储名字后续显示
    Ui::showpdfForm *ui;
};

#endif // SHOWPDFFORM_H
