#include "formpdf.h"
#include "ui_formpdf.h"

FormPdf::FormPdf(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormPdf)
{
    ui->setupUi(this);
    setWindowTitle("查看器");
    this->setAttribute(Qt::WA_DeleteOnClose);
}

FormPdf::~FormPdf()
{
    QMessageBox::information(this,"信息","PDF被关闭");
    delete ui;
}

void FormPdf::loadpdf(QImage &pdfimg)
{
    ui->label->setPixmap(QPixmap::fromImage(pdfimg));
}
