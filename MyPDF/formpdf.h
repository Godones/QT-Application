#ifndef FORMPDF_H
#define FORMPDF_H

#include "pagerender.h"
#include <QDebug>
#include <QImage>
#include <QLabel>
#include <QMessageBox>
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
    void pagechanged(int currentpage);

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
};

#endif // FORMPDF_H
