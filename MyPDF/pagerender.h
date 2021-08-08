#ifndef PAGERENDER_H
#define PAGERENDER_H

#include<QImage>
#include<QThread>
#include<poppler-qt5.h>

class PageRender :public QThread
{
public:
    explicit PageRender(QObject*parent = nullptr);

signals:
    void pageReady(int page,qreal zoom,QImage image);//页面已经准备好的信号
public slots:
    void setDocument(Poppler::Document * document);//读取文档
    void requestPage(int page,qreal zoom,Priority = QThread::NormalPriority);

protected:
    void run();

private:
    void Renderpage(int page);
    int c_page;
    qreal c_zoom;
    Poppler::Document * c_document;
};

#endif // PAGERENDER_H
