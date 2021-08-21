#include "pdftable.h"
#include "ui_pdftable.h"

PdfTable::PdfTable(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::PdfTable)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose); //关闭销毁
    init_set();
    setMouseTracking(true); //开启鼠标追踪功能
    PageRender = new TitlepageRender(this);
    connectSlot();
}

PdfTable::~PdfTable()
{
    delete ui;
    //    qDebug() << "TAbleDelete";
}

void PdfTable::connectSlot()
{
    //连接信号
    connect(PageRender, &TitlepageRender::pagerendered, this, &PdfTable::createtable);
}

void PdfTable::init_set()
{
    //初始化设置
    //设置显示模式，只显示图标
    ui->listWidget->setViewMode(QListView::IconMode);
    //设置图标大小
    ui->listWidget->setIconSize(QSize(wideCover, heightCover));
    //设置间距
    ui->listWidget->setSpacing(10);
    //设置自适应布局
    ui->listWidget->setResizeMode(QListWidget::Adjust);
    //不能移动
    ui->listWidget->setMovement(QListWidget::Static);

    ui->listWidget->setFont(QFont("微软雅黑"));
    //无焦点
    ui->listWidget->setFocusPolicy(Qt::NoFocus);
    ui->listWidget->setFrameShape(QFrame::NoFrame);
    ui->listWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listWidget->setSelectionMode(QAbstractItemView::NoSelection);
    //    ui->listWidget->setTextElideMode(Qt::ElideNone);
}

void PdfTable::init_read(QString path)
{
    //显示path文件夹下的所有pdf文件
    respdir = path;

    QDir dir(path);
    QString filtername = "*.pdf";
    QStringList filterlist;
    filterlist << filtername;
    dir.setNameFilters(filterlist);
    QStringList allpdf = dir.entryList(); //获取所有的文件

    //QMimeDatabase db;//判断文件类型
    //QMimeType mime = db.mimeTypeForFile(allpdf[i]);
    //if(mime.name()!="application/pdf") continue;

    for (int i = 0; i < allpdf.size(); i++) {
        QString path = dir.absoluteFilePath(allpdf[i]);
        name.push_back(path);
    }

    PageRender->set_data(name);
}

void PdfTable::createtable(QPixmap titlepage, QString okpdfdir)
{

    QFileInfo info;

    QListWidgetItem* item = new QListWidgetItem;
    item->setIcon(QIcon(titlepage));
    item->setSizeHint(QSize(titlepage.width(), titlepage.height() + 50));
    item->setData(Qt::UserRole, QVariant(okpdfdir));
    info.setFile(okpdfdir);
    item->setText(info.fileName());
    item->setToolTip(info.fileName());
    //        item->setTextAlignment(Qt::AlignCenter);
    item->setTextAlignment(Qt::AlignJustify);
    ui->listWidget->addItem(item);
}

void PdfTable::receive_information_addfile(QStringList pdffiles)
{
    //接收来自主窗口增加文件发射的信号;
    auto dir = pdffiles.toVector();
    PageRender->set_data(dir);
}

void PdfTable::on_listWidget_itemEntered(QListWidgetItem* item)
{
    if (item == NULL) {
        return;
    }
    QToolTip::showText(QCursor::pos(), item->text());
}

void PdfTable::on_listWidget_itemDoubleClicked(QListWidgetItem* item)
{
    //双击cell事件
    //打开文件
    QString pdfpath = item->data(Qt::UserRole).toString();
    emit sentfilepath(pdfpath); //发送自定义信号给主窗口
}
