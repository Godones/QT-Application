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
    QString Pdfname;
    void load(QString pdfpath,QTreeWidget*);

    void updatePDF();
    void fitwindowshow();
    void fitpageshow();
    void getRect(int& ,int&);
    QImage getImage(int index);
    void setscale(int _scale);

    void get_xml_Marks(Poppler::Document* pdfdoc,QTreeWidget* );
    void read_xml(QDomNode, QTreeWidgetItem*,QTreeWidget*  );
private:
    Ui::AllPageShow *ui;
    QVector<Poppler::Page *> allpage;//存储所有页面
    QString Pdfpath;//存储PDF路径
    QSize size;
    bool IsFullRender;//是否全部渲染完成


    int scale;
    int minRender,maxRender;
    QGraphicsScene *myscene;
    const int onceRender = 1;//每次渲染视图中央的页面上下三个
    const int firtRender = 3;
    void init_set();

    void Silderchange();

};

#endif // ALLPAGESHOW_H
