#include "titlepagerender.h"

TitlepageRender::TitlepageRender(QObject* parent)
    : QThread(parent)
{
}

void TitlepageRender::set_data(QVector<QString>& dir, Priority)
{
    Pdfpath.clear();
    for (QString x : dir) {
        Pdfpath.append(x);
    }
    start(priority());
}

void TitlepageRender::run()
{
    render();
}

void TitlepageRender::render()
{
    QVector<QPixmap> titilepage;
    QVector<QString> okpdfdir;
    Poppler::Document* document;

    for (auto x : Pdfpath) {
        document = Poppler::Document::load(x);
        if (document == nullptr)
            continue;

        document->setRenderBackend(Poppler::Document::SplashBackend);
        document->setRenderHint(Poppler::Document::Antialiasing);
        document->setRenderHint(Poppler::Document::TextAntialiasing);
        document->setRenderHint(Poppler::Document::ThinLineShape);
        QSizeF psize = document->page(0)->pageSizeF();

        qreal h_zoom = heightCover / psize.height();
        qreal w_zoom = wideCover / psize.width();

        QPixmap pix = QPixmap::fromImage(document->page(0)->renderToImage(
            72.0 * w_zoom, 72 * h_zoom, 0, 0, psize.width() * w_zoom, psize.height() * h_zoom));

        emit pagerendered(pix, x);
    }
}
