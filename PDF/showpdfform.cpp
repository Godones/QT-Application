#include "showpdfform.h"
#include "ui_showpdfform.h"


showpdfForm::showpdfForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::showpdfForm)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);//关闭销毁
    connectSlot();
}

void showpdfForm::connectSlot()
{
    //连接信号

}

void showpdfForm::receive_information_addfile(QStringList pdffiles)
{
    //接收来自主窗口增加文件发射的信号;

    qDebug()<<pdffiles.size();

    Poppler::Document *pdfdoc=nullptr;
    QMimeDatabase db;
    for(int i=0;i<pdffiles.size();i++){
        pdfdoc = Poppler::Document::load(pdffiles[i]);//只添加可以打开的pdf文件
        if(pdfdoc!=nullptr){
            QImage image= pdfdoc->page(0)->renderToImage();
            if(image.isNull()) continue;
            ico.push_back(image);
            name.push_back(pdffiles[i]);
        }
    }
    createtable();
}

showpdfForm::~showpdfForm()
{
    delete ui;
}

void showpdfForm::init(QString &path)
{
    //显示path文件夹下的所有pdf文件
    QDir dir(path);
    QStringList allpdf = dir.entryList(QDir::Files);//获取所有的文件
    Poppler::Document *pdfdoc=nullptr;
    QMimeDatabase db;





    for(int i=0;i<allpdf.size();i++){
        QMimeType mime = db.mimeTypeForFile(allpdf[i]);
        if(mime.name()!="application/pdf") continue;
//        qDebug()<<dir.absoluteFilePath(allpdf[i]);
        pdfdoc = Poppler::Document::load(dir.absoluteFilePath(allpdf[i]));//只添加可以打开的pdf文件

        pdfdoc->setRenderBackend(Poppler::Document::SplashBackend);
        pdfdoc->setRenderHint(Poppler::Document::Antialiasing);
        pdfdoc->setRenderHint(Poppler::Document::TextAntialiasing);
        pdfdoc->setRenderHint(Poppler::Document::ThinLineShape);


        if(pdfdoc!=nullptr){
            QSize size = pdfdoc->page(0)->pageSize();
            float weightScale = (float)wideCover/size.width();
            float heightScale = (float)heightCover/size.height();
            qDebug()<<weightScale<<"!!!"<<heightScale<<size;
            QImage image= pdfdoc->page(0)->renderToImage(72*weightScale,72*heightScale,0,0,wideCover,heightCover);
            if(image.isNull()) continue;
            ico.push_back(image);
            name.push_back(dir.absoluteFilePath(allpdf[i]));
        }
    }
    createtable();
}

void showpdfForm::createtable()
{


    if(ico.isEmpty()) return;
    //设置滚轮
    ui->tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

//    //设置行列标题的可见性
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setVisible(false);
//    //不可编辑
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    //字体
    ui->tableWidget->setFont(QFont("楷体"));
    ui->tableWidget->setShowGrid(false);


    //获取table宽度
    int wide = this->geometry().size().width();

    qDebug()<<ui->tableWidget->size()<<"@@@";

    //列数 wide = wideGap*(x+1) + wideCover*x，x为每一行放多少张封面
    int columnneed = (wide - wideGap)/(wideGap+wideCover);
    //行数
    int rowneed = ico.size()/columnneed+1;



    //设置行列总数，此处需要小心处理
    ui->tableWidget->setColumnCount(columnneed*2+1);
    ui->tableWidget->setRowCount(rowneed*2);



    for(int i=0;i<columnneed*2+1;i++)
        if(i%2==0) ui->tableWidget->setColumnWidth(i,wideGap);
        else ui->tableWidget->setColumnWidth(i,wideCover);
    for(int i=0;i<2*rowneed;i++)
        if(i%2==0) ui->tableWidget->setRowHeight(i,heightCover);
        else ui->tableWidget->setRowHeight(i,heightGap);

    int k = 0;
    QFileInfo info;
    for(int i=0;i<ui->tableWidget->rowCount();i++){
        if(i%2==1)continue;
        for(int j=0;j<ui->tableWidget->columnCount();j++){
            if(j%2==0)continue;
            if(k>=ico.size()) return;
            QLabel* label = new QLabel;
            label->setPixmap(QPixmap::fromImage(ico[k]));


            QTableWidgetItem *item = new QTableWidgetItem;
            item->setData(Qt::UserRole,QVariant(name[k]));
            ui->tableWidget->setItem(i,j,item);
            ui->tableWidget->setCellWidget(i,j,label);

            info.setFile(name[k]);
            QTableWidgetItem *nameitem = new QTableWidgetItem(info.fileName());
            nameitem->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);//文字对齐方式
            ui->tableWidget->setItem(i+1,j,nameitem);

            k++;
        }
    }



}

void showpdfForm::resizeEvent(QResizeEvent *event)
{
    //调整窗口大小时需要重新布局排列
    createtable();
}


void showpdfForm::on_tableWidget_cellDoubleClicked(int row, int column)
{
    //双击cell事件
    //打开文件
//    QMessageBox::information(this,"","test!!!!!");
    QString pdfpath = ui->tableWidget->item(row,column)->data(Qt::UserRole).toString();
    qDebug()<<pdfpath;
    emit sentfilepath(pdfpath);//发送自定义信号给主窗口
}

