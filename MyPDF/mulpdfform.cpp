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

    //    vlayout->addWidget(area);
    //    vlayout->setSpacing(0);
    //    vlayout->setContentsMargins(0, 0, 0, 0);

    this->setAttribute(Qt::WA_DeleteOnClose);
    splitter = new QSplitter(this);
    dock = new QDockWidget(this);
    treeWidget = new QTreeWidget(this);
    dock->setWidget(treeWidget);
    dock->setMaximumWidth(400);
    dock->setFeatures(QDockWidget::DockWidgetClosable);
    dock->setWindowTitle("书签");

    splitter->addWidget(dock);
    splitter->addWidget(area);

    hlayout = new QHBoxLayout(this);
    hlayout->addWidget(splitter);
    hlayout->setSpacing(0);
    hlayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(hlayout);

    zoom = 1.0;
    currentpage = 0;
    numpages = 0;
    connect(PDF, &MulPage::updateinfo, this, &MulPDFForm::updateinfo);
    connect(treeWidget, &QTreeWidget::itemClicked, this, &MulPDFForm::treeWidget_itemClicked);
}

MulPDFForm::~MulPDFForm()
{
    delete ui;
}

void MulPDFForm::updateinfo(int current, int totalpages, qreal _zoom)
{
    Q_UNUSED(totalpages);
    zoom = _zoom;
    currentpage = current;
    numpages = totalpages;
    emit pagechanged(current, totalpages, zoom); //显示当前页面的信号
}

void MulPDFForm::nextpage()
{
    PDF->nextpage();
    area->verticalScrollBar()->setValue(PDF->yForPage());
}

void MulPDFForm::prepage()
{
    PDF->prepage();
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

void MulPDFForm::fitpageshow()
{
    //适合页面显示
    PDF->fitpageshow();
    area->verticalScrollBar()->setValue(PDF->yForPage());
}

void MulPDFForm::fitwindowshow()
{
    PDF->fitwindowsshow();
    area->verticalScrollBar()->setValue(PDF->yForPage());
}

void MulPDFForm::locatepage(int index)
{
    PDF->locatepage(index);
    area->verticalScrollBar()->setValue(PDF->yForPage());
}

bool MulPDFForm::openPDF(QString PDFpath)
{
    PdfPath = PDFpath;
    if (PDF->setDocument(PDFpath)) {
        XmlLoad xmlload(PDFpath);
        xmlload.get_xml_Marks(treeWidget);
        treeWidget->expandAll();
        return true;
    }
    return false;
}

void MulPDFForm::mouseMoveEvent(QMouseEvent* event)
{
    //未开启鼠标奥追踪的情况下，当鼠标按下时才能触发
    if (mouse_is_press) {
        int currentval_y = area->verticalScrollBar()->value() + y - event->y();
        //鼠标向下移动，则y < event.y,页面向上移动
        area->verticalScrollBar()->setValue(currentval_y);
        y = event->y();
        int currentval_x = area->horizontalScrollBar()->value() + x - event->x();
        area->horizontalScrollBar()->setValue(currentval_x);
        x = event->x();
    }
}

void MulPDFForm::mousePressEvent(QMouseEvent* event)
{
    mouse_is_press = true;
    y = event->y();
    x = event->x();
}

void MulPDFForm::mouseReleaseEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    mouse_is_press = false;
}
void MulPDFForm::treeWidget_itemClicked(QTreeWidgetItem* item, int column)
{
    //点击书签，跳转到指定位置
    Q_UNUSED(column);
    int index = item->data(0, Qt::UserRole).toInt();
    locatepage(index);
}
