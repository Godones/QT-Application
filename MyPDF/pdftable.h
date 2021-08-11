#ifndef PDFTABLE_H
#define PDFTABLE_H

#include "titlepagerender.h"
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

    TitlepageRender* PageRender;

public:
    explicit PdfTable(QWidget* parent = nullptr);
    ~PdfTable();

    //读取文件
    void init_read(QString path);
    //初始设置
    void init_set();

    void createtable(QPixmap, QString);

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
    const qreal wideCover = 220; //展示封面大小
    const qreal heightCover = 300;
};

#endif // PDFTABLE_H
