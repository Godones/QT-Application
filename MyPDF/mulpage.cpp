#include "mulpage.h"
#include "dialogpdf.h"

MulPage::MulPage(QWidget* parent)
    : QWidget(parent)
{
    m_pageCacheLimit = 10;
    m_PageRender = new PageRender();
    m_pageSpacing = 4; //设置页面间隔8
    m_pageIndex = 0;
    m_totalPages = 0;
    m_totalSize = QSize(0, 0);
    m_zoom = 1.0;
    m_document = NULL;

    connect(m_PageRender, SIGNAL(pageReady(int, qreal, QImage)), this,
        SLOT(pageLoaded(int, qreal, QImage)), Qt::QueuedConnection);
    // 排队连接：
    // 当控制返回到接收者线程的事件循环时，将调用该插槽。该槽在接收者的线程中执行。
    grabGesture(Qt::SwipeGesture); //启用滑动手势
}

MulPage::~MulPage() { delete m_PageRender; }

bool MulPage::setDocument(const QString& filePath)
{
    qDebug() << "INN";
    m_document = Poppler::Document::load(filePath);
    if (m_document == nullptr)
        return false;

    m_document->setRenderBackend(Poppler::Document::SplashBackend);
    m_document->setRenderHint(Poppler::Document::Antialiasing);
    m_document->setRenderHint(Poppler::Document::TextAntialiasing);
    m_document->setRenderHint(Poppler::Document::ThinLineShape);
    numpages = m_document->numPages();
    m_PageRender->setDocument(m_document); //注入线程信息
    m_totalPages = m_document->numPages(); //获取页面数量
    for (int i = 0; i < m_totalPages; i++) {
        m_pageSizes.append(m_document->page(i)->pageSize()); //页面原大小
    }
    invalidate();
    return true;
}

int MulPage::yForPage()
{
    //获取当前页面所在的位置
    int y = m_pageSpacing;
    for (int i = 0; i < m_pageIndex && i < m_totalPages; i++) {
        y += m_pageSpacing + pageSize(i).height();
    }
    return y;
}

QImage MulPage::getPDFImage(int index)
{
    QImage c_image;
    if (index >= 0 && index < m_totalPages) {
        Poppler::Page* tpage = m_document->page(index);
        QSizeF tsize = tpage->pageSizeF();
        c_image = tpage->renderToImage(72 * m_zoom, 72 * m_zoom, 0, 0,
            tsize.width() * m_zoom, tsize.height() * m_zoom);
    }
    return c_image;
}

void MulPage::nextpage()
{
    if (m_pageIndex < m_totalPages - 1)
        m_pageIndex++;
}

void MulPage::prepage()
{
    if (m_pageIndex > 0)
        m_pageIndex--;
}

void MulPage::zoomIn()
{
    //    放大
    if (m_zoom < 5.0f) {
        m_zoom += 0.1f;
        invalidate(); //更新页面，重置缓冲区等
    }
}

void MulPage::zoomOut()
{
    if (m_zoom > 0.1f) {
        m_zoom -= 0.1f;
        invalidate();
    }
}

void MulPage::locatepage(int page)
{
    //跳转到某一页
    if (page > 0 && page < m_totalPages) {
        m_pageIndex = page;
    }
}

void MulPage::pageLoaded(int page, qreal zoom, QImage image)
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

    update(); //更新窗口，此处需要处理一下防止闪烁
}

void MulPage::invalidate()
{
    QSizeF totalSize(0, m_pageSpacing);
    QSizeF size(0, 0);
    for (int page = 0; page < m_totalPages; ++page) {
        size = pageSize(page); //获取页面的大小
        totalSize.setHeight(totalSize.height() + size.height() + m_pageSpacing);
        if (size.width() > totalSize.width()) {
            totalSize.setWidth(size.width());
        }
    }
    totalSize += QSizeF(1, 1);
    m_totalSize = totalSize.toSize();

    setMinimumSize(m_totalSize); //设置窗口最小大小
    qDebug() << "pagesize: " << m_totalSize << " " << m_totalPages;
    m_pageCache.clear();
    m_cachedPagesLRU.clear();
    update(); //更新窗口
}

void MulPage::paintEvent(QPaintEvent* event)
{
    //重绘窗口
    QPainter painter(this);
    if (m_totalPages == 0) {
        return;
        //没有页面时，不用重绘
    }
    //找到第一张要绘制的画面
    int t_top = event->rect().top();
    int t_bottom = event->rect().bottom();
    int page = 0;
    int y = m_pageSpacing;

    int t_vtop = this->visibleRegion().boundingRect().top();
    int t_vbottom = this->visibleRegion().boundingRect().bottom();

    while (page < m_totalPages) {
        int t_height = pageSize(page).height();
        if (y + t_height + m_pageSpacing >= t_vtop)
            break;
        y += t_height + m_pageSpacing;
        page++;
    }

    while (y <= t_vbottom && page < m_totalPages) {
        //在可视窗口内渲染pdf
        QSizeF tsize = pageSize(page); //图片大小

        if (m_pageCache.contains(page)) {
            //如果已经被缓存了直接使用即可
            QImage timage = m_pageCache[page]; //获取图像
            painter.fillRect((width() - timage.width()) / 2, y, tsize.width(),
                tsize.height(), Qt::white);

            //            qDebug()<<"tsize与timage应该一样大小"<<tsize<<timage.size();

            qDebug() << "有" << QRect((width() - timage.width()) / 2, y, tsize.width(), tsize.height());

            painter.setRenderHint(QPainter::Antialiasing, true); //抗锯齿
            painter.drawPixmap((width() - timage.width()) / 2, y, QPixmap::fromImage(timage));
            getPage(t_vtop, t_vbottom);
            emit updateinfo(page, m_totalPages, m_zoom, numpages); //更新内容

        } else {
            //缓存中不存在
            painter.fillRect((width() - tsize.width()) / 2, y, tsize.width(),
                tsize.height(), Qt::white);

            if (!m_PageRender->isRunning())
                m_PageRender->requestPage(page, m_zoom); //加载pdf页面
        }
        ++page;
        y += (tsize.height() + m_pageSpacing);
    }
}

int MulPage::getPage(int t_vtop, int t_vbottom)
{
    //获取视图中央的页面索引
    int y = 0, page = 0;
    int center = (t_vtop + t_vbottom) / 2;
    for (page = 0; page < m_totalPages; page++) {
        int t_size = m_pageSpacing + pageSize(page).toSize().height();
        if (y == t_vtop) {
            m_pageIndex = page;
            break;
        }
        if (y < t_vtop && y + t_size >= t_vbottom) {
            //窗口视图较小
            m_pageIndex = page;
            break;
        }
        if (y > t_vtop && y + t_size <= t_vbottom) {
            m_pageIndex = page;
            break;
        }
        if (y <= center && y + t_size > t_vbottom) {
            m_pageIndex = page;
            break;
        }
        if (y > center && y + t_size > t_vbottom) {
            m_pageIndex = page - 1;
            break;
        }
    }
    return m_pageIndex;
}

QSizeF MulPage::pageSize(int page)
{
    //返回页面大小
    return m_pageSizes[page] * m_zoom;
}
