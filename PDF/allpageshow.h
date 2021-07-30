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
#include<QDomDocument>
#include<QDomElement>
#include<QTreeWidget>

namespace Ui {
class AllPageShow;
}

class AllPageShow : public QWidget
{
    Q_OBJECT

public:
    explicit AllPageShow(QWidget *parent = nullptr);
    ~AllPageShow();

    bool load();

    void updatePDF();

    void fitpageshow();

    QImage getImage(int index);

    void setscale(int _scale);

    void located(int index);

    QString Pdfpath;//存储PDF路径

    int currentpage;//当前所在页面

    int scale;//缩放因子

    int numpages;

    Poppler::Document *pdfdoc ;

signals:
    void pagechanged(int currentpage);

private:
    Ui::AllPageShow *ui;
    QVector<Poppler::Page *> allpage;//存储所有页面


    QSize size;


    int minRender,maxRender;

    QGraphicsScene *myscene;

    const int onceRender = 1;//每次渲染视图中央的页面上下三个

    const int firtRender = 3;


    void Silderchange();

};

#endif // ALLPAGESHOW_H
