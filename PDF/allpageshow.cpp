#include "allpageshow.h"
#include "ui_allpageshow.h"
#include<math.h>



AllPageShow::AllPageShow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AllPageShow)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    myscene = new QGraphicsScene;

//    connect(ui->graphicsView->verticalScrollBar(),&QAbstractSlider:: sliderReleased,this,&AllPageShow::updatePDF);

}

AllPageShow::~AllPageShow()
{
    delete ui;
    qDebug()<<"AllPageDelete";
}

bool AllPageShow::load()
{
    //加载pdf文件
    pdfdoc = Poppler::Document::load(Pdfpath);
    if(pdfdoc==nullptr) return false;
    //PDf大小
    size = pdfdoc->page(0)->pageSize();
    //初始放缩
    scale = 100;
    numpages = pdfdoc->numPages();

    if(pdfdoc!=nullptr){
        pdfdoc->setRenderBackend(Poppler::Document::SplashBackend);
        pdfdoc->setRenderHint(Poppler::Document::Antialiasing);
        pdfdoc->setRenderHint(Poppler::Document::TextAntialiasing);
        pdfdoc->setRenderHint(Poppler::Document::ThinLineShape);
        for(int i=0;i<pdfdoc->numPages();i++){
            Poppler::Page *page = pdfdoc->page(i);//获得所有页面
            allpage.push_back(page);
        }
        fitpageshow();
    }
    return true;
}

void AllPageShow::updatePDF()
{
    //新方法，根据滚动滑轮位置来调整渲染的画面
    QScrollBar *bar = ui->graphicsView->verticalScrollBar();

    qDebug()<<ui->graphicsView->size()<<bar->value()<<size<<"UPDATE"<<myscene->sceneRect()<<bar->size();

    QTransform t;
    int minindex,maxindex;

    float scaled = (float)scale/100.0;
    minindex = (bar->value()+bar->height()/2)/(size.height()*scaled);

    maxindex = minindex;

    if(currentpage!=minindex) emit pagechanged(minindex);//当前页改变后，触发信号
    else return;

    currentpage = minindex;

    int minneed = minindex-onceRender;
    if(minneed<0) minneed = 0;
    int maxneed = maxindex+onceRender;
    if(maxneed>=allpage.size()) maxneed = allpage.size()-1;


    for(int i = minneed;i<=maxneed;i++){
        QGraphicsItem *item = myscene->itemAt(0,i*size.height()*scale,t);
        if(item==nullptr||item->data(Qt::UserRole+2).toInt()!=scale){
            if(item!=nullptr)
                myscene->removeItem(item);
            QImage image = getImage(i);
            QGraphicsPixmapItem* item = myscene->addPixmap(QPixmap::fromImage(image));
            item->setPos(0,size.height()*i*scaled);
            item->setData(Qt::UserRole,QVariant(i));
            item->setData(Qt::UserRole+2,QVariant( scale));
        }
    }
}


void AllPageShow::fitpageshow(){
    //适合页面
    QGraphicsItem* need = nullptr;

    float scaled = (float)scale/100;
    myscene->setSceneRect(0,0,size.width()*scaled,size.height()*scaled*allpage.size());

    for(int i=0;i<allpage.size();i++){
        if(i<firtRender){
            QImage image = getImage(i);
            QGraphicsPixmapItem* item = myscene->addPixmap(QPixmap::fromImage(image));
            item->setPos(0,size.height()*i);
            item->setData(Qt::UserRole,QVariant(i));
            item->setData(Qt::UserRole+2,QVariant(scale));
            if(i==0) need = item;
        }
    }
    minRender = 0,maxRender = firtRender-1;
    currentpage = 0;
    ui->graphicsView->setScene(myscene);
    ui->graphicsView->centerOn(need);
    connect(ui->graphicsView,&Mygraphics::updatePDF,this,&AllPageShow::updatePDF);
}


QImage AllPageShow::getImage(int index)
{
    //获取index的Qimage
    float scale_ =(float) scale/100;
    QImage image = allpage[index]->renderToImage(72*scale_,72*scale_,0,index,size.width()*scale_,size.height()*scale_);
    return image;
}

void AllPageShow::setscale(int _scale)
{
    //重新绘制画面，将之前渲染的全部删除，渲染滚动条对应位置的即可
    scale = _scale;
    QScrollBar *bar = ui->graphicsView->verticalScrollBar();

    qDebug()<<ui->graphicsView->size()<<bar->value()<<size<<"Scene: "<<myscene->sceneRect()<<bar->rect()<<allpage.size();

    QTransform t;

    int barheight = bar->size().height()/2;
    QGraphicsItem *item =  myscene->itemAt(0,bar->value()+barheight,t);//获取滚轮位置的图片
    int address = item->data(Qt::UserRole).toInt();

//    qDebug()<<"FLAG"<<address;

    //断开更新操作信号连接
    disconnect(ui->graphicsView,&Mygraphics::updatePDF,this,&AllPageShow::updatePDF);
    //调整场景大小
    //先清空场景内的内容
    myscene->clear();
    float scaled = (float)scale/100;

    myscene->setSceneRect(0,0,(int)(size.width()*scaled),(int)(size.height()*scaled*allpage.size()));


    qDebug()<<ui->graphicsView->size()<<bar->value()<<size*scaled<<"Scene: "<<myscene->sceneRect()<<bar->rect()<<allpage.size();


    QGraphicsPixmapItem* newitem = myscene->addPixmap(QPixmap::fromImage(getImage(address)));
    newitem->setPos(0,size.height()*address*scaled);
    newitem->setData(Qt::UserRole,QVariant(address));
    newitem->setData(Qt::UserRole+2,QVariant(scale));

    ui->graphicsView->centerOn(newitem);

    currentpage = address;


    connect(ui->graphicsView,&Mygraphics::updatePDF,this,&AllPageShow::updatePDF);


}

void AllPageShow::located(int index)
{
    //定位到某一页
    float scaled = (float)scale/100;
    ui->graphicsView->centerOn(0,size.height()*index*scaled);
    currentpage = index;
    emit pagechanged(currentpage);//
    updatePDF();
}



void AllPageShow::Silderchange()
{
    //Test
    qDebug()<<"NEW";
}
