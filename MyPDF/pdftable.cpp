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
    connectSlot();
}

PdfTable::~PdfTable()
{
    delete ui;
    qDebug() << "TAbleDelete";
}

void PdfTable::connectSlot()
{
    //连接信号
    //    connect(ui->listWidget,SIGNAL(itemEntered(QTableWidgetItem *)),this,SLOT(on_listWidget_itemEntered(QListWidgetItem *)));
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

QImage PdfTable::getImage(QString& filepath)
{
    //获得pdf封面大小
    Poppler::Document* pdfdoc = Poppler::Document::load(filepath); //只添加可以打开的pdf文件
    if (pdfdoc != nullptr) {
        pdfdoc->setRenderBackend(Poppler::Document::SplashBackend);
        pdfdoc->setRenderHint(Poppler::Document::Antialiasing);
        pdfdoc->setRenderHint(Poppler::Document::TextAntialiasing);
        pdfdoc->setRenderHint(Poppler::Document::ThinLineShape);
        QSize size = pdfdoc->page(0)->pageSize();
        float weightScale = (float)wideCover / size.width();
        float heightScale = (float)heightCover / size.height();
        QImage image = pdfdoc->page(0)->renderToImage(72 * weightScale, 72 * heightScale, 0, 0, wideCover, heightCover);
        if (!image.isNull())
            return image;
    }
}
void PdfTable::init_read(QString& path)
{
    //显示path文件夹下的所有pdf文件
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
        QImage image = getImage(path);
        ico.push_back(image);
        name.push_back(path);
    }
    QFileInfo info(path);
    //    setWindowTitle(info.fileName());
    createtable();
}

void PdfTable::createtable()
{

    QFileInfo info;

    qDebug() << ico.size();
    for (int i = 0; i < ico.size(); i++) {
        QListWidgetItem* item = new QListWidgetItem;
        item->setIcon(QIcon(QPixmap::fromImage(ico[i])));
        item->setSizeHint(QSize(wideCover, heightCover + 50));
        item->setData(Qt::UserRole, QVariant(name[i]));
        info.setFile(name[i]);
        item->setText(info.fileName());
        item->setToolTip(info.fileName());
        //        item->setTextAlignment(Qt::AlignCenter);
        item->setTextAlignment(Qt::AlignJustify);
        ui->listWidget->addItem(item);
    }
}

void PdfTable::receive_information_addfile(QStringList pdffiles)
{
    //接收来自主窗口增加文件发射的信号;
    QFileInfo info;
    for (int i = 0; i < pdffiles.size(); i++) {
        QImage image = getImage(pdffiles[i]);
        QListWidgetItem* item = new QListWidgetItem;
        item->setIcon(QIcon(QPixmap::fromImage(image)));
        item->setSizeHint(QSize(wideCover, heightCover + 30));
        item->setData(Qt::UserRole, QVariant(pdffiles[i]));
        info.setFile(pdffiles[i]);
        item->setText(info.fileName());
        item->setToolTip(info.fileName());
        //        item->setTextAlignment(Qt::AlignCenter);
        item->setTextAlignment(Qt::AlignJustify);
        ui->listWidget->addItem(item);
    }
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
    //    QMessageBox::information(this,"","test!!!!!");
    QString pdfpath = item->data(Qt::UserRole).toString();
    emit sentfilepath(pdfpath); //发送自定义信号给主窗口
}
