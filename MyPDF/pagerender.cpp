#include "pagerender.h"

PageRender::PageRender(QObject* parent)
    : QThread(parent)
    , c_page(0)
    , c_zoom(1.0)
    , c_document(NULL)
{
}

void PageRender::setDocument(Poppler::Document* document)
{
    c_document = document;
}

void PageRender::requestPage(int page, qreal zoom, Priority)
{
    c_page = page;
    c_zoom = zoom;
    start(priority());
}

void PageRender::run()
{
    Renderpage(c_page); //开启任务进程
}

void PageRender::Renderpage(int page)
{
    if (c_document == nullptr)
        return;
    Poppler::Page* tpage = c_document->page(page);
    QSizeF sizepage = tpage->pageSizeF();
    QImage image = tpage->renderToImage(72 * c_zoom, 72 * c_zoom, 0, 0,
        sizepage.width() * c_zoom,
        sizepage.height() * c_zoom);
    emit pageReady(page, c_zoom, image);
}
