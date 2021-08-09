#include "mulpdfform.h"
#include "ui_mulpdfform.h"

MulPDFForm::MulPDFForm(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::MulPDFForm)
{
    mouse_is_press = false;

    ui->setupUi(this);
    PDF = new MulPage;
    area = new QScrollArea;
    area->setAlignment(Qt::AlignCenter);

    area->setWidgetResizable(true);
    area->setWidget(PDF);
    vlayout = new QVBoxLayout;

    vlayout->addWidget(area);

    this->setAttribute(Qt::WA_DeleteOnClose);
    this->setLayout(vlayout);
}

MulPDFForm::~MulPDFForm()
{
    delete ui;
}

void MulPDFForm::nextpage()
{
    PDF->prepage();
    area->verticalScrollBar()->setValue(PDF->yForPage());
}

void MulPDFForm::prepage()
{
    PDF->nextpage();
    area->verticalScrollBar()->setValue(PDF->yForPage());
}

void MulPDFForm::zoomIn()
{
    PDF->zoomIn();
    area->verticalScrollBar()->setValue(PDF->yForPage());
}

void MulPDFForm::zoomOut()
{
    PDF->zoomOut();
    area->verticalScrollBar()->setValue(PDF->yForPage());
}

void MulPDFForm::locatepage(int index)
{
    PDF->locatepage(index);
    area->verticalScrollBar()->setValue(PDF->yForPage());
}

void MulPDFForm::openPDF(QString& PDFpath)
{
    PDF->setDocument(PDFpath);
}

void MulPDFForm::mouseMoveEvent(QMouseEvent* event)
{
    //未开启鼠标奥追踪的情况下，当鼠标按下时才能触发
    if (mouse_is_press) {
        int currentval = area->verticalScrollBar()->value() + y - event->y();
        //鼠标向下移动，则y < event.y,页面向上移动
        area->verticalScrollBar()->setValue(currentval);
        y = event->y();

        qDebug() << PDF->size() << area->size();
    }
}

void MulPDFForm::mousePressEvent(QMouseEvent* event)
{
    mouse_is_press = true;
    y = event->y();
}

void MulPDFForm::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    mouse_is_press = false;
}
