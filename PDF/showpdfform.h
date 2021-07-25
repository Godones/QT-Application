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
    void createtable();
    void resizeEvent(QResizeEvent *event) override;
    void connectSlot();
    void receive_information_addfile(QStringList pdffiles);//槽函数接受主窗口的增加文件事件
    QImage getImage(QString &filepath);



signals:
    void sentfilepath(QString pdfpath);


private slots:

    void on_tableWidget_cellDoubleClicked(int row, int column);

private:
    QVector<QImage>ico;//存储所有能打开的pdf图标
    QVector<QString>name;//存储名字后续显示
    const int wideCover = 200;   //展示封面大小
    const int heightCover = 300;
    const int wideGap = 10;
    const int heightGap = 80;


    Ui::showpdfForm *ui;
};

#endif // SHOWPDFFORM_H
