#ifndef FORMPDF_H
#define FORMPDF_H

#include "pagerender.h"
#include "xmlload.h"
#include <QDebug>
#include <QImage>
#include <QLabel>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QScrollBar>
#include <QSizeF>
#include <QWidget>
#include <poppler-qt5.h>
namespace Ui {
class FormPdf;
}

class FormPdf : public QWidget {
    Q_OBJECT

public:
    explicit FormPdf(QWidget* parent = nullptr);
    ~FormPdf();

public:
    QString PdfPath; //pdf路径
    int currentpage; //当前页面
    qreal m_zoom; //zoom系数
    int numpages;
    //四个变量需要在切换页面显示时使用

    bool loadpdf();

signals:
    void pagechanged(int currentpage, int totalpages, qreal zoom);

public slots:

    void fitwindowshow();
    void fitpageshow();
    void nextpage();
    void prepage();
    void locatepage(int);

    void pageLoaded(int page, qreal zoom, QImage image);

    void zoomIn();
    void zoomOut();

private:
    Ui::FormPdf* ui;

    Poppler::Document* pdfdoc;

    PageRender* m_PageRender; //线程渲染

    QHash<int, QImage> m_pageCache; //页面缓存

    QVector<int> m_cachedPagesLRU;

    int m_pageCacheLimit; //缓存限制

    QSizeF getpagesize(int index);

    void show();

protected:
    int x;
    int y;
    bool mouse_is_press; //鼠标被按下
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

    void wheelEvent(QWheelEvent* event) override;
private slots:

    void on_treeWidget_itemClicked(QTreeWidgetItem* item, int column);
};

#endif // FORMPDF_H
