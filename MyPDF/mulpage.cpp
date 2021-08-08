#include "mulpage.h"

MulPage::MulPage(QWidget *parent) : QWidget(parent)
{
    m_pageCacheLimit = 10;
    m_PageRender = new PageRender();
    m_pageSpacing = 4; //设置页面间隔8
    m_pageIndex = 0;
    m_totalPages = 0;
    m_totalSize = QSize(0,0);
    m_zoom = 1.0;
    m_document = nullptr;
    
    connect(m_PageRender, SIGNAL(pageReady(int, qreal, QImage)), this, SLOT(pageLoaded(int, qreal, QImage)), Qt::QueuedConnection);
    // 排队连接： 当控制返回到接收者线程的事件循环时，将调用该插槽。该槽在接收者的线程中执行。
    grabGesture(Qt::SwipeGesture);//启用滑动手势
}

MulPage::~MulPage()
{
    delete m_PageRender;
}



bool MulPage::setDocument(const QString &filePath)
{
    m_document = Poppler::Document::load(filePath);
    if (m_document==nullptr) return false;

    m_document->setRenderBackend(Poppler::Document::SplashBackend);
    m_document->setRenderHint(Poppler::Document::Antialiasing);
    m_document->setRenderHint(Poppler::Document::TextAntialiasing);
    m_document->setRenderHint(Poppler::Document::ThinLineShape);


    m_PageRender->setDocument(m_document);//注入线程信息
    m_totalPages = m_document->numPages();//获取页面数量
    for(int i=0;i<m_totalPages;i++){
        m_pageSizes.append(m_document->page(i)->pageSizeF());//页面原大小
    }
    invalidate();
    return true;
}


int MulPage::yForPage()
{
    
}

QImage MulPage::getPDFImage(int index)
{
    QImage c_image;
    if(index>=0&&index<m_totalPages){
        Poppler::Page *tpage = m_document->page(index);
        QSize tsize = tpage->pageSize();
        c_image =tpage->renderToImage(72*m_zoom,72*m_zoom,0,index,tsize.width()*m_zoom,tsize.height()*m_zoom);
    }
    return c_image;
}

void MulPage::pageLoaded(int page, qreal zoom, QImage image)
{
    //已经渲染好，可以使用
    Q_UNUSED(zoom);
    if(m_cachedPagesLRU.length()>m_pageCacheLimit){
        //缓存的图片超过限制
        m_pageCache.remove(m_cachedPagesLRU.takeFirst());//移除
    }
    m_pageCache.insert(page,image);
    m_cachedPagesLRU.append(page);
    update();//更新窗口，此处需要处理一下防止闪烁
}

void MulPage::invalidate()
{
    //根据页面大小数量等设置窗口大小并更新
    qreal height_all = m_pageSpacing;//最顶上的间隔
    qreal wide_max = 0;
    for(int i=0;i<m_totalPages;i++){
        QSizeF tsize = pageSize(i);
        height_all +=tsize.height()+m_pageSpacing;
        if(tsize.width()>wide_max) wide_max = tsize.width();
    }
    height_all +=2;
    wide_max +=2;
    m_totalSize = QSizeF(wide_max,height_all).toSize();
    setMinimumSize(m_totalSize);
    m_pageCache.clear();
    update();//更新窗口，调用重绘事件
    
}

void MulPage::paintEvent(QPaintEvent *event)
{
    //重绘窗口
    QPainter painter(this);
    if(m_totalPages==0){
        return;
        //没有页面时，不用重绘
    }
    //找到第一张要绘制的画面
    int t_top = event->rect().top();
    int t_bottom = event->rect().bottom();
    int page = 0;
    int y= m_pageSpacing;
    
    int t_vtop = this->visibleRegion().boundingRect().top();
    int t_vbottom = this->visibleRegion().boundingRect().bottom();
    
    while (page<m_totalPages) {
        int t_height = pageSize(page).height();
        if(y+t_height>=t_vtop) break;
        y += t_height + m_pageSpacing;
        page++;
    }


    while (y<=t_vbottom&&page<m_totalPages) {
        //在可视窗口内渲染pdf
        QSizeF tsize = pageSize(page); //图片大小
        
        if(m_pageCache.contains(page)){
            //如果已经被缓存了直接使用即可
            QImage timage = m_pageCache[page];//获取图像
            painter.fillRect(width()-timage.width()/2,y,tsize.width(),tsize.height(),Qt::white);
            
            //            qDebug()<<"tsize与timage应该一样大小"<<tsize<<timage.size();
            painter.drawPixmap(width()-timage.width()/2,y,QPixmap::fromImage(timage));
            getPage( t_vtop,t_vbottom);
            emit updateinfo(page,m_totalPages,m_zoom);
        }
        else {
            //缓存中不存在
            painter.fillRect(width()-tsize.width()/2,y,tsize.width(),tsize.height(),Qt::white);
            
            m_PageRender->requestPage(page,m_zoom);//加载pdf页面
            
        }
        ++page;
        y +=tsize.height()+m_pageSpacing;
    }
    
}

int MulPage::getPage(int t_vtop,int t_vbottom)
{
    //获取视图中央的页面索引
    int y = 0,page = 0;
    int center = (t_vtop+t_vbottom)/2;
    for(page = 0;page<m_totalPages;page++){
        int t_size = m_pageSpacing+pageSize(page).toSize().height();
        if(y==t_vtop){
            m_pageIndex = page;
            break;
        }
        if(y<t_vtop&& y+t_size>=t_vbottom){
            //窗口视图较小
            m_pageIndex = page;
            break;
        }
        if(y>t_vtop&&y+t_size<=t_vbottom){
            m_pageIndex = page;
            break;
        }
        if(y<=center&& y+t_size>t_vbottom){
            m_pageIndex = page;
            break;
        }
        if(y>center&&y+t_size>t_vbottom){
            m_pageIndex = page -1;
            break;
        }
    }
    return m_pageIndex;
    
}
QSizeF MulPage::pageSize(int page)
{
    //返回页面大小
    return m_pageSizes[page]*m_zoom;
}
