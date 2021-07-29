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

bool FormPdf::loadpdf()
{
    pdfdoc = Poppler::Document::load(PdfPath);

    if(pdfdoc!=nullptr){
        pdfdoc->setRenderBackend(Poppler::Document::SplashBackend);
        pdfdoc->setRenderHint(Poppler::Document::Antialiasing);
        pdfdoc->setRenderHint(Poppler::Document::TextAntialiasing);
        pdfdoc->setRenderHint(Poppler::Document::ThinLineShape);

        currentpage = 0;
        scaled = 100;
        fitpageshow();
        return true;
    }
    else return false;

}

void FormPdf::fitwindowshow()
{
    //PDF以适合窗口显示
    //PDF的宽度是窗口的宽度，高度按宽度的比例缩放
    QSize size = ui->scrollArea->size();
    QSize pagesize = pdfdoc->page(currentpage)->pageSize();
    float scale =(float) size.width()/ pagesize.width();
    currentshow = pdfdoc->page(currentpage)->renderToImage(72*scale,72*scale,0,currentpage,pagesize.width()*scale,pagesize.height()*scale);
    ui->label->setPixmap(QPixmap::fromImage(currentshow));
}

void FormPdf::fitpageshow()
{
    //PDF以适合页面显示
    QSize size = pdfdoc->page(currentpage)->pageSize();//页面大小
    currentshow = pdfdoc->page(currentpage)->renderToImage(72,72,0,currentpage,size.width(),size.height());
    ui->label->setPixmap(QPixmap::fromImage(currentshow));//显示

}

void FormPdf::scale(int factor)
{
    float factor_ = (float)factor/100;
    scaled = factor;
    QSize size = pdfdoc->page(currentpage)->pageSize();//页面大小
    currentshow = pdfdoc->page(currentpage)->renderToImage(72*factor_,72*factor_,0,currentpage,size.width()*factor_,factor_*size.height());
    ui->label->setPixmap(QPixmap::fromImage(currentshow));//显示

}

void FormPdf::nextview()
{
    //下一页
    if(currentpage<pdfdoc->numPages()-1) {

        float factor_ = (float)scaled/100;
        currentpage +=1;
        QSize size = pdfdoc->page(currentpage)->pageSize();//页面大小
        currentshow = pdfdoc->page(currentpage)->renderToImage(72*factor_,72*factor_,0,currentpage,size.width()*factor_,factor_*size.height());
        ui->label->setPixmap(QPixmap::fromImage(currentshow));
    }
    else return;
}

void FormPdf::preview()
{
    //上一页
    if(currentpage>0) {
        float factor_ = (float)scaled/100;
        currentpage -=1;
        QSize size = pdfdoc->page(currentpage)->pageSize();//页面大小
        currentshow = pdfdoc->page(currentpage)->renderToImage(72*factor_,72*factor_,0,currentpage,size.width()*factor_,factor_*size.height());
        ui->label->setPixmap(QPixmap::fromImage(currentshow));
    }
    else return;
}

void FormPdf::located(int num)
{
    //定位
    if(num>=0&&num<pdfdoc->numPages()){
        float factor_ = (float)scaled/100;
        currentpage = num;
        QSize size = pdfdoc->page(currentpage)->pageSize();//页面大小
        currentshow = pdfdoc->page(currentpage)->renderToImage(72*factor_,72*factor_,0,currentpage,size.width()*factor_,factor_*size.height());
        ui->label->setPixmap(QPixmap::fromImage(currentshow));
    }
}

