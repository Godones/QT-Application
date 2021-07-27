#ifndef ALLPAGESHOW_H
#define ALLPAGESHOW_H

#include <QWidget>
#include<QListView>
#include<QVector>
#include<poppler-qt5.h>
#include<QStandardItem>
#include<QPixmap>
#include<QtDebug>
#include<QStandardItemModel>
#include<QGraphicsScene>
#include<QGraphicsView>
#include<QGraphicsItem>
#include<mygraphics.h>

namespace Ui {
class AllPageShow;
}

class AllPageShow : public QWidget
{
    Q_OBJECT

public:
    explicit AllPageShow(QWidget *parent = nullptr);
    ~AllPageShow();

    void load(QString pdfpath);

    void updatePDF();
    void fitwindowshow();
    void fitpageshow();
private:
    Ui::AllPageShow *ui;
    QVector<Poppler::Page *> allpage;//存储所有页面
    QString Pdfpath;//存储PDF路径
    QStandardItemModel *model;

    int currentRender;//当前渲染项
    QGraphicsScene *myscene;
    const int onceRender = 4;


    void init_set();
};

#endif // ALLPAGESHOW_H
