#include "dialogpdf.h"
#include "ui_dialogpdf.h"

DialogPDF::DialogPDF(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogPDF)
{
    ui->setupUi(this);
    ui->label->setAlignment(Qt::AlignCenter);
}

DialogPDF::~DialogPDF()
{
    delete ui;
}
void DialogPDF::load(QPixmap pix)
{
    ui->label->setPixmap(pix);
}
