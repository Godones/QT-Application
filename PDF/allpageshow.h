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
#include<QTransform>
#include<mygraphics.h>
#include<QScrollBar>
#include<QSize>


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
    void getRect(int& ,int&);
    QImage getImage(int index,float scale = 1.0);


private:
    Ui::AllPageShow *ui;
    QVector<Poppler::Page *> allpage;//存储所有页面
    QString Pdfpath;//存储PDF路径
    QStandardItemModel *model;
    QSize size;


    int minRender,maxRender;
    QGraphicsScene *myscene;
    const int onceRender = 4;//每次渲染视图中央的页面上下三个
    const int firtRender = 3;
    void init_set();


};

#endif // ALLPAGESHOW_H
