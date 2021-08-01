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
    //不适用，弃用方法
    QScrollBar *bar = ui->graphicsView->verticalScrollBar();
    QTransform t;

    //     测试

    updateShow();
   QSize viewsize = ui->graphicsView->size();
   QPointF center =  ui->graphicsView->mapToScene(viewsize.width()/2,viewsize.height()/2);
   QGraphicsItem* centeritem =  myscene->itemAt(center,t);
   qDebug()<<centeritem->data(Qt::UserRole).toInt()<<centeritem->data(Qt::UserRole+2).toInt()<<center<<viewsize;
   if(centeritem!=nullptr) qDebug()<<"Test: "<<centeritem->scenePos();


    int minindex,maxindex;
    float scaled = (float)scale/100.0;
    minindex = (bar->value()+bar->height()/2)/(size.height()*scaled);
    maxindex = minindex;

    qDebug()<<ui->graphicsView->size()<<bar->value()<<size*scaled<<"UPDATE"<<myscene->sceneRect()<<bar->size();

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

void AllPageShow::updateShow(){

    QTransform t;
    //获取视图大小
   QSize viewsize = ui->graphicsView->size();
   //获取视图中央位置在场景中的位置
   QPointF centerneed =  ui->graphicsView->mapToScene(viewsize.width()/2,viewsize.height()/2);

   float scaled = (float)scale/100;
   //对应于第y张图片
   int y = (float)centerneed.ry()/(size.height()*scaled);

   qDebug()<<y<<"~~~~~~~~~"<<centerneed.ry();


   int hold =(float) viewsize.height()/((size.height()*scaled));

   int over = (y+onceRender)>=numpages?numpages-1:y+onceRender;
   myscene->clear();
   for(int i= (y-onceRender)<0?0:y-onceRender ;i<= over;i++){
       qDebug()<<i;
       QImage image = getImage(i);
       QGraphicsPixmapItem* item = myscene->addPixmap(QPixmap::fromImage(image));
       item->setPos(0,size.height()*i*scaled);
       item->setData(Qt::UserRole,QVariant(i));
       item->setData(Qt::UserRole+2,QVariant( scale));
   }


}

void AllPageShow::fitpageshow(){
    //适合页面
    QGraphicsItem* need = nullptr;

    float scaled = (float)scale/100;
    myscene->setSceneRect(0,0,size.width()*scaled,size.height()*scaled*allpage.size());

    //设置视图大小
//    ui->graphicsView->setSceneRect(myscene->sceneRect());

    for(int i=0;i<allpage.size();i++){
        if(i<firtRender){
            QImage image = getImage(i);
            QGraphicsPixmapItem* item = myscene->addPixmap(QPixmap::fromImage(image));
            item->setPos(0,size.height()*i*scaled);
            item->setData(Qt::UserRole,QVariant(i));
            item->setData(Qt::UserRole+2,QVariant(scale));            
            if(i==0) need = item;
            //更新isRendered
        }
    }
    currentpage = 0;
    ui->graphicsView->setScene(myscene);
    ui->graphicsView->centerOn(need);

//    connect(ui->graphicsView,&Mygraphics::updatePDF,this,&AllPageShow::updatePDF);
    connect(ui->graphicsView,&Mygraphics::updatePDF,this,&AllPageShow::updateShow);

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
    //重新绘制画面，将之前渲染的全部删除
    scale = _scale;
    float scaled = (float)scale/100;
    QSize viewsize = ui->graphicsView->size();
    //获取视图中央位置在场景中的位置
    QPointF centerneed =  ui->graphicsView->mapToScene(viewsize.width()/2,viewsize.height()/2);

    //对应于第y张图片
    int y = (float)centerneed.ry()/(size.height()*scaled);

    QTransform t;

    //断开更新操作信号连接
//    disconnect(ui->graphicsView,&Mygraphics::updatePDF,this,&AllPageShow::updatePDF);
    disconnect(ui->graphicsView,&Mygraphics::updatePDF,this,&AllPageShow::updateShow);

    //调整场景大小
    //先清空场景内的内容
    myscene->clear();
    myscene->setSceneRect(0,0,(int)(size.width()*scaled),(int)(size.height()*scaled*allpage.size()));
    qDebug()<<ui->graphicsView->size()<<size*scaled<<"Scene: "<<myscene->sceneRect()<<allpage.size();

    QGraphicsPixmapItem* newitem = myscene->addPixmap(QPixmap::fromImage(getImage(y)));
    newitem->setPos(0,size.height()*y*scaled);
    newitem->setData(Qt::UserRole,QVariant(y));
    newitem->setData(Qt::UserRole+2,QVariant(scale));

    ui->graphicsView->centerOn(newitem);

    currentpage = y;

//    connect(ui->graphicsView,&Mygraphics::updatePDF,this,&AllPageShow::updatePDF);
    connect(ui->graphicsView,&Mygraphics::updatePDF,this,&AllPageShow::updateShow);

}

void AllPageShow::located(int index)
{
    //定位到某一页
    float scaled = (float)scale/100;
    ui->graphicsView->centerOn(0,size.height()*(float)(index+0.5)*scaled);
    currentpage = index;
    emit pagechanged(currentpage);//
//    updatePDF();
    updateShow();

}



void AllPageShow::Silderchange()
{
    //Test
    qDebug()<<"NEW";
}
