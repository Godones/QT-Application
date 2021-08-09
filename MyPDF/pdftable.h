#ifndef PDFTABLE_H
#define PDFTABLE_H

#include <QAbstractItemView>
#include <QDebug>
#include <QDir>
#include <QImage>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QToolTip>
#include <QWidget>
#include <poppler-qt5.h>

namespace Ui {
class PdfTable;
}

class PdfTable : public QWidget {
    Q_OBJECT

private:
    QVector<QImage> ico; //存储所有能打开的pdf图标
    QVector<QString> name; //存储名字后续显示
    const int wideCover = 220; //展示封面大小
    const int heightCover = 300;
    const int wideGap = 10;
    const int heightGap = 80;

public:
    explicit PdfTable(QWidget* parent = nullptr);
    ~PdfTable();

    //读取文件
    void init_read(QString path);
    //初始设置
    void init_set();
    QImage getImage(QString& filepath);
    void createtable();
    void receive_information_addfile(QStringList pdffiles);

    QString respdir;

signals:
    void sentfilepath(QString pdfpath);

private slots:
    void on_listWidget_itemEntered(QListWidgetItem* item);

    void on_listWidget_itemDoubleClicked(QListWidgetItem* item);

private:
    Ui::PdfTable* ui;

    void connectSlot();
};

#endif // PDFTABLE_H
