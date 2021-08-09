#include "formpdf.h"
#include "ui_formpdf.h"
#include <QScreen>

FormPdf::FormPdf(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::FormPdf)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->label->setAlignment(Qt::AlignCenter); //居中显示
    m_PageRender = new PageRender(this);
    connect(m_PageRender, &PageRender::pageReady, this, &FormPdf::pageLoaded, Qt::QueuedConnection);
}

FormPdf::~FormPdf()
{
    delete ui;
    qDebug() << "FormPdfDelete";
}

bool FormPdf::loadpdf()
{
    pdfdoc = Poppler::Document::load(PdfPath);
    if (pdfdoc != nullptr) {
        pdfdoc->setRenderBackend(Poppler::Document::SplashBackend);
        pdfdoc->setRenderHint(Poppler::Document::Antialiasing);
        pdfdoc->setRenderHint(Poppler::Document::TextAntialiasing);
        pdfdoc->setRenderHint(Poppler::Document::ThinLineShape);
        currentpage = 0;
        m_zoom = 1.0;
        m_pageCacheLimit = 10;
        numpages = pdfdoc->numPages();
        m_PageRender->setDocument(pdfdoc);
        show();
        return true;
    } else
        return false;
}

void FormPdf::pageLoaded(int page, qreal zoom, QImage image)
{
    //已经渲染好，可以使用
    Q_UNUSED(zoom);
    if (m_cachedPagesLRU.length() > m_pageCacheLimit) {
        //缓存的图片超过限制
        m_pageCache.remove(m_cachedPagesLRU.takeFirst()); //移除
    }
    m_pageCache.insert(page, image);
    m_cachedPagesLRU.append(page);

    //    DialogPDF* temp = new DialogPDF(this);
    //    temp->load(QPixmap::fromImage(image));
    //    temp->show();

    if (page == currentpage) {
        //如果当前页面还是渲染的页面，则重新显示
        show();
    }
}
void FormPdf::show()
{
    if (m_pageCache.contains(currentpage)) {
        //缓存中存在则直接使用
        ui->label->clear();
        ui->label->setPixmap(QPixmap::fromImage(m_pageCache[currentpage]));
        ui->scrollArea->verticalScrollBar()->setValue(0); //回到页面起始位置
        emit pagechanged(currentpage);
    } else {
        //缓存中不存在则需要先在子进程中加载
        m_PageRender->requestPage(currentpage, m_zoom);
    }
}

void FormPdf::zoomIn()
{
    //    放大
    if (m_zoom < 5.0f) {
        m_zoom += 0.1f;
        m_pageCache.clear();
        m_cachedPagesLRU.clear();
        show();
    }
}

void FormPdf::zoomOut()
{
    if (m_zoom > 0.1f) {
        m_zoom -= 0.1f;
        m_pageCache.clear();
        m_cachedPagesLRU.clear();
        show();
    }
}

void FormPdf::fitwindowshow()
{
    // PDF以适合窗口显示
    // PDF的宽度是窗口的宽度，高度按宽度的比例缩放
    QSize size = ui->scrollArea->size();
    qreal zoom = (float)size.width() / getpagesize(currentpage).width();
    if (zoom != m_zoom) {
        m_zoom = zoom;
        m_pageCache.clear();
        m_cachedPagesLRU.clear();
        show();
    }
}

void FormPdf::fitpageshow()
{
    // PDF以适合页面显示
    if (m_zoom != 1.0f) {
        //当前页面的zoom不等于1才重新显示
        m_zoom = 1.0;
        m_pageCache.clear();
        m_cachedPagesLRU.clear();
        show();
    }
}

void FormPdf::nextpage()
{
    //下一页
    if (currentpage < pdfdoc->numPages() - 1) {
        currentpage += 1;
        show();

    } else
        return;
}

void FormPdf::prepage()
{
    //上一页
    if (currentpage > 0) {
        currentpage -= 1;
        show();
    } else
        return;
}

void FormPdf::locatepage(int num)
{
    //定位
    if (num >= 0 && num < pdfdoc->numPages()) {
        currentpage = num;
        show();
    }
}

QSizeF FormPdf::getpagesize(int index)
{
    //返回页面大小
    return pdfdoc->page(index)->pageSizeF();
}
