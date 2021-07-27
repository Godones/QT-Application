#include "allpageshow.h"
#include "ui_allpageshow.h"
#include<math.h>

AllPageShow::AllPageShow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AllPageShow)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    setWindowTitle("展示");
//    init_set();
    myscene = new QGraphicsScene;

    connect(ui->graphicsView,&Mygraphics::updatePDF,this,&AllPageShow::updatePDF);

}

AllPageShow::~AllPageShow()
{
    delete ui;
}
/*
void AllPageShow::init_set()
{
    //初始化设置
    //设置显示模式，只显示图标
    ui->listWidget->setViewMode(QListView::IconMode);
    //设置图标大小
//    ui->listWidget->setIconSize(QSize(wideCover,heightCover));
    //设置间距
//    ui->listWidget->setSpacing(10);
    //设置自适应布局
    ui->listWidget->setResizeMode(QListWidget::Adjust);
    //不能移动
    ui->listWidget->setMovement(QListWidget::Static);

    ui->listWidget->setFont(QFont("微软雅黑"));
    //无焦点
    ui->listWidget->setFocusPolicy(Qt::NoFocus);
    ui->listWidget->setFrameShape(QFrame::NoFrame);
    ui->listWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listWidget->setSelectionMode(QAbstractItemView::NoSelection);
//    ui->listWidget->setTextElideMode(Qt::ElideNone);

}

*/

void AllPageShow::load(QString pdfpath)
{
    //加载pdf文件
    Pdfpath = pdfpath;
    Poppler::Document* pdfdoc = Poppler::Document::load(pdfpath);
//    QRect mrect;//获取设备分辨率
//    mrect =QGuiApplication::primaryScreen()->geometry();
//    qDebug()<<Poppler::Document::availableRenderBackends();//获取渲染后端

    if(pdfdoc!=nullptr){
        pdfdoc->setRenderBackend(Poppler::Document::SplashBackend);
        pdfdoc->setRenderHint(Poppler::Document::Antialiasing);
        pdfdoc->setRenderHint(Poppler::Document::TextAntialiasing);
        pdfdoc->setRenderHint(Poppler::Document::ThinLineShape);
        for(int i=0;i<pdfdoc->numPages();i++){
            Poppler::Page *page = pdfdoc->page(i);//获得所有页面
            allpage.push_back(page);
        }
//        setWindowTitle(Pdfname);
        fitpageshow();
    }

}

void AllPageShow::updatePDF()
{
    //获取当前视图框
    //获取里面的图形项,并找到最下方的一个，判断是否没有被渲染，如果未被渲染，则渲染
    QRect showrect= ui->graphicsView->rect();
    QList<QGraphicsItem *>item =  ui->graphicsView->items(showrect);
    int maxnum = 0;
    for(auto &x:item){
        if(x->data(Qt::UserRole).toInt()>maxnum) maxnum = x->data(Qt::UserRole).toInt();
    }
    QSize size = allpage[0]->pageSize();
    if(qAbs(maxnum-currentRender)<2){
        for(int i=currentRender;i<currentRender+onceRender&&i<allpage.size();i++){
            QImage image = allpage[i]->renderToImage(72,72,0,i,size.width(),size.height());
            QGraphicsPixmapItem* item =   myscene->addPixmap(QPixmap::fromImage(image));
            item->setOffset(0,size.height()*i);
        }
        currentRender = currentRender+onceRender;
    }

}
void AllPageShow::fitwindowshow(){

    //PDF的宽度是窗口的宽度，高度按宽度的比例缩放
    QSize size = ui->graphicsView->size();
    QSize pagesize = allpage[0]->pageSize();
    float scale =(float) size.width()/ pagesize.width();

    for(int i=0;i<allpage.size();i++){
        QImage image = allpage[i]->renderToImage(72*scale,72*scale,0,i,pagesize.width()*scale,pagesize.height()*scale);
    }

}

void AllPageShow::fitpageshow(){
    //适合页面
    QSize size = allpage[0]->pageSize();//页面大小

    for(int i=0;i<allpage.size();i++){
        if(i<onceRender){
            QImage image = allpage[i]->renderToImage(72,72,0,i,size.width(),size.height());
            QGraphicsPixmapItem* item =   myscene->addPixmap(QPixmap::fromImage(image));
            item->setOffset(0,size.height()*i);
            item->setData(Qt::UserRole,QVariant(i));
        }
        else {
            QPixmap temp = QPixmap(size);
            QGraphicsPixmapItem* item =   myscene->addPixmap(temp);
            item->setOffset(0,size.height()*i);
            item->setData(Qt::UserRole,QVariant(i));
        }
        currentRender = onceRender;
    }
    ui->graphicsView->setScene(myscene);
}
