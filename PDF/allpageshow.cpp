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

//    connect(ui->graphicsView->verticalScrollBar(),&QAbstractSlider:: sliderReleased,this,&AllPageShow::updatePDF);
}

AllPageShow::~AllPageShow()
{
    delete ui;
}

void AllPageShow::load(QString pdfpath,QTreeWidget* mainTree)
{
    //加载pdf文件
    Pdfpath = pdfpath;
    Poppler::Document* pdfdoc = Poppler::Document::load(pdfpath);
    size = pdfdoc->page(0)->pageSize();
    scale = 100;

    get_xml_Marks(pdfdoc,mainTree);
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

        //设置滚动栏范围
        fitpageshow();
    }

}
void AllPageShow::getRect(int& minindex,int& maxindex)
{

    QRect showrect= ui->graphicsView->rect();
    QList<QGraphicsItem *>item =  ui->graphicsView->items(showrect);
    minindex = item[0]->data(Qt::UserRole).toInt();
    maxindex = item[item.size()-1]->data(Qt::UserRole).toInt();

}
void AllPageShow::updatePDF()
{
    //获取当前视图框
    //获取里面的图形项,加上前后几页,判断是否没有被渲染，如果未被渲染，则渲染

    //新方法，根据滚动滑轮位置来调整渲染的画面
    QScrollBar *bar = ui->graphicsView->verticalScrollBar();

//    qDebug()<<ui->graphicsView->size()<<bar->value()<<size<<"UPDATE"<<myscene->sceneRect()<<bar->size();

    QTransform t;
    int minindex,maxindex;

    float scaled = (float)scale/100.0;
    minindex = (bar->value()+bar->height()/2)/(size.height()*scaled);

    maxindex = minindex;
    int minneed = minindex-onceRender;
    if(minneed<0) minneed = 0;
    int maxneed = maxindex+onceRender;
    if(maxneed>=allpage.size()) maxneed = allpage.size()-1;

    if(minneed>=minRender&&maxneed<=maxRender) return;

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
    minRender = minneed,maxRender = maxneed;
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
    qDebug()<<ui->graphicsView->size()<<bar->value()<<size<<"Scene: "<<myscene->sceneRect()<<bar->rect();

    QTransform t;

    int barheight = bar->size().height()/2;
    QGraphicsItem *item =  myscene->itemAt(0,bar->value()+barheight,t);//获取滚轮位置的图片
    int address = item->data(Qt::UserRole).toInt();

    qDebug()<<"FLAG"<<address;

    //断开更新操作信号连接
    disconnect(ui->graphicsView,&Mygraphics::updatePDF,this,&AllPageShow::updatePDF);
    //调整场景大小
    //先清空场景内的内容
    myscene->clear();
    float scaled = (float)scale/100;
    myscene->setSceneRect(0,0,size.width()*scaled,size.height()*scaled*allpage.size());

    QGraphicsPixmapItem* newitem = myscene->addPixmap(QPixmap::fromImage(getImage(address)));
    newitem->setPos(0,size.height()*address*scaled);
    newitem->setData(Qt::UserRole,QVariant(address));
    newitem->setData(Qt::UserRole+2,QVariant(scaled));

    ui->graphicsView->centerOn(newitem);


    minRender = maxRender = address;

    connect(ui->graphicsView,&Mygraphics::updatePDF,this,&AllPageShow::updatePDF);


}

void AllPageShow::get_xml_Marks(Poppler::Document *pdfdoc,QTreeWidget *mainTree)
{

    QDomDocument* doc = pdfdoc->toc();
    if(doc==nullptr) return;

    mainTree->setColumnCount(1);
    mainTree->setWindowTitle("书签");
    mainTree->setSizeAdjustPolicy(QTreeWidget::AdjustToContents);
    //获取第一个节点
    qDebug()<<doc->toString();

    QDomNode node = doc->firstChild();
    while (!node.isNull()) {
        QDomElement e = node.toElement();
        QTreeWidgetItem *item  = new QTreeWidgetItem();
        item->setData(0,Qt::UserRole,e.attribute("Destination"));
        item->setText(0,e.tagName());
        mainTree->addTopLevelItem(item);
        if(node.hasChildNodes())
            read_xml(node.firstChild(),item,mainTree);
        node = node.nextSibling();
    }
}

void AllPageShow::read_xml(QDomNode node, QTreeWidgetItem * parent,QTreeWidget* mainTree)
{

     // try to convert the node to an element.
    while (!node.isNull()) {
        QDomElement e = node.toElement();
        if (!e.isNull())
        {
           qDebug() << e.tagName()<<e.attribute("Destination"); // the node really is an element.
           QTreeWidgetItem *item  = new QTreeWidgetItem();
           item->setData(0,Qt::UserRole,e.attribute("Destination"));
           item->setText(0,e.tagName());
           if(parent==nullptr)
                mainTree ->addTopLevelItem(item);
           else parent->addChild(item);

           if(e.hasChildNodes()) {

               read_xml(e.firstChild(),item,mainTree);
           }
        }
        node = node.nextSibling();
    }

    return;
}

void AllPageShow::Silderchange()
{
    //Test
    qDebug()<<"NEW";
}
