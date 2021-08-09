#ifndef MULPAGE_H
#define MULPAGE_H

#include <QDebug>
#include <QGuiApplication>
#include <QPaintEvent>
#include <QPainter>
#include <QScreen>
#include <QWidget>
#include <pagerender.h>

class MulPage : public QWidget {
    Q_OBJECT
public:
    explicit MulPage(QWidget* parent = nullptr);
    ~MulPage();

    void paintEvent(QPaintEvent* event); //重写绘图事件
    bool setDocument(const QString& filePath); //获取文档
    int getPage(int, int); //获取页面索引
    int yForPage(); //页面的y值大小
    QImage getPDFImage(int index); //获取图片

signals:
    void updateinfo(int pageindex, int totalpage, qreal zoom); //更新信息

public slots:
    void nextpage();
    void prepage();
    void zoomIn(); //放大
    void zoomOut(); //缩小
    void locatepage(int); //跳转
    void fitpageshow(); //适合页面
    void fitwindowsshow();

private slots:
    void pageLoaded(int page, qreal zoom, QImage image);

private:
    PageRender* m_PageRender; //线程渲染

    QHash<int, QImage> m_pageCache; //页面缓存

    QVector<int> m_cachedPagesLRU;

    int m_pageCacheLimit; //缓存限制

    QVector<QSizeF> m_pageSizes; //各个页面大小

    int m_pageSpacing; //页面间隔

    int m_pageIndex; //页面索引

    int m_totalPages; //总页面数

    QSize m_totalSize; //总大小

    qreal m_zoom; //缩放大小

    Poppler::Document* m_document;

    void invalidate();

    QSizeF pageSize(int page);
};

#endif // MULPAGE_H
