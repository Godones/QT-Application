#include "formpdf.h"
#include "ui_formpdf.h"
#include <QScreen>

FormPdf::FormPdf(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormPdf)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->label->setAlignment(Qt::AlignCenter); //居中显示
}

FormPdf::~FormPdf()
{
    delete ui;
}

void FormPdf::loadpdf()
{


    pdfdoc = Poppler::Document::load(PdfPath);
//    QRect mrect;//获取设备分辨率
//    mrect =QGuiApplication::primaryScreen()->geometry();
//    qDebug()<<Poppler::Document::availableRenderBackends();//获取渲染后端

    pdfdoc->setRenderBackend(Poppler::Document::SplashBackend);
    pdfdoc->setRenderHint(Poppler::Document::Antialiasing);
    pdfdoc->setRenderHint(Poppler::Document::TextAntialiasing);
    pdfdoc->setRenderHint(Poppler::Document::ThinLineShape);
    qDebug()<<pdfdoc->pageLayout();
    if(pdfdoc!=nullptr){
        QSize size = pdfdoc->page(0)->pageSize();//页面大小
        qDebug()<<size<<" "<<pdfdoc->numPages();
        currentpage = 0;
        currentshow= pdfdoc->page(0)->renderToImage(72,72,0,currentpage,size.width(),size.height());
    }
    setWindowTitle(Pdfname);
    fitwindowshow();
}

void FormPdf::fitwindowshow()
{
    //PDF以适合页面显示
    QSize size = pdfdoc->page(0)->pageSize();//页面大小
    currentshow = pdfdoc->page(currentpage)->renderToImage(72,72,0,currentpage,size.width(),size.height());
    ui->label->setPixmap(QPixmap::fromImage(currentshow));//显示

}

void FormPdf::scale(int factor)
{
    float factor_ = (float)factor/100;
    QSize size = pdfdoc->page(0)->pageSize();//页面大小
    currentshow = pdfdoc->page(currentpage)->renderToImage(72*factor_,72*factor_,0,currentpage,size.width()*factor_,factor_*size.height());
    ui->label->setPixmap(QPixmap::fromImage(currentshow));//显示

}

void FormPdf::nextview()
{
    if(currentpage<pdfdoc->numPages()-1) {
        currentpage +=1;
        QSize size = pdfdoc->page(currentpage)->pageSize();//页面大小
        currentshow = pdfdoc->page(currentpage)->renderToImage(72,72,0,currentpage,size.width(),size.height());
        ui->label->setPixmap(QPixmap::fromImage(currentshow));
        fitwindowshow();
    }
    else return;
}

void FormPdf::preview()
{
    if(currentpage>0) {
        currentpage -=1;
        currentshow = pdfdoc->page(currentpage)->renderToImage();
        ui->label->setPixmap(QPixmap::fromImage(currentshow));
        fitwindowshow();
    }
    else return;
}

void FormPdf::located(int num)
{
    if(num>=0&&num<pdfdoc->numPages()){
        currentpage = num;
        currentshow = pdfdoc->page(num)->renderToImage();
        ui->label->setPixmap(QPixmap::fromImage(currentshow));
        fitwindowshow();
    }
}

