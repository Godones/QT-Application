#include "showpdfform.h"
#include "ui_showpdfform.h"


showpdfForm::showpdfForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::showpdfForm)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);//关闭销毁

    //设置滚轮
    ui->tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
   //设置行列标题的可见性
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setVisible(false);
     //不可编辑
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //不可选中
    ui->tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
    //字体
    ui->tableWidget->setFont(QFont("微软雅黑"));
    ui->tableWidget->setShowGrid(false);
    //没有焦点，点击不会显示边框
    ui->tableWidget->setFocusPolicy(Qt::NoFocus);
    //无边框
    ui->tableWidget->setFrameShape(QFrame::NoFrame);



    connectSlot();
}

void showpdfForm::connectSlot()
{
    //连接信号

}


QImage showpdfForm::getImage(QString& filepath)
{
    //获得pdf封面大小
    Poppler::Document *pdfdoc = Poppler::Document::load(filepath);//只添加可以打开的pdf文件
    if(pdfdoc!=nullptr){
        pdfdoc->setRenderBackend(Poppler::Document::SplashBackend);
        pdfdoc->setRenderHint(Poppler::Document::Antialiasing);
        pdfdoc->setRenderHint(Poppler::Document::TextAntialiasing);
        pdfdoc->setRenderHint(Poppler::Document::ThinLineShape);

        QSize size = pdfdoc->page(0)->pageSize();
        float weightScale = (float)wideCover/size.width();
        float heightScale = (float)heightCover/size.height();
        QImage image= pdfdoc->page(0)->renderToImage(72*weightScale,72*heightScale,0,0,wideCover,heightCover);
        if(!image.isNull()) return image;
    }

}
void showpdfForm::receive_information_addfile(QStringList pdffiles)
{
    //接收来自主窗口增加文件发射的信号;

    for(int i=0;i<pdffiles.size();i++){
        QImage image = getImage(pdffiles[i]);
        ico.push_back(image);
        name.push_back(pdffiles[i]);
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
    QString filtername = "*.pdf";
    QStringList filterlist;
    filterlist<<filtername;
    dir.setNameFilters(filterlist);
    QStringList allpdf = dir.entryList();//获取所有的文件

    //QMimeDatabase db;//判断文件类型
    //QMimeType mime = db.mimeTypeForFile(allpdf[i]);
    //if(mime.name()!="application/pdf") continue;
    for(int i=0;i<allpdf.size();i++){
        QString path = dir.absoluteFilePath(allpdf[i]);
        QImage image = getImage(path);
        ico.push_back(image);
        name.push_back(path);
    }
    QFileInfo info(path);
    setWindowTitle(info.fileName());
    createtable();
}

void showpdfForm::createtable()
{


    if(ico.isEmpty()) return;
    //获取table宽度
    int wide = this->geometry().size().width();
    qDebug()<<ui->tableWidget->size()<<"@@@";

    //列数 wide = wideGap*(x+1) + wideCover*x，x为每一行放多少张封面
    int columnneed = (wide - wideGap)/(wideGap+wideCover)-1;
    //行数
    int rowneed = ico.size()/columnneed+1;
    //设置行列总数，此处需要小心处理
    ui->tableWidget->setColumnCount(columnneed*2+1);
    ui->tableWidget->setRowCount(rowneed*2);


    for(int i=0;i<columnneed*2+1;i++)
        if(i%2==0) ui->tableWidget->resizeColumnToContents(i);
        else ui->tableWidget->setColumnWidth(i,wideCover);
    for(int i=0;i<2*rowneed;i++)
        if(i%2==0) ui->tableWidget->setRowHeight(i,heightCover);
        else ui->tableWidget->resizeRowToContents(i);
    int k = 0;
    QFileInfo info;
    for(int i=0;i<ui->tableWidget->rowCount();i++){
        if(i%2==1)continue;
        for(int j=0;j<ui->tableWidget->columnCount();j++){
            if(j%2==0)continue;

            if(k>=ico.size()) {
                //剩余的用空白填充

            }
            else{

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



}

void showpdfForm::resizeEvent(QResizeEvent *event)
{
    //调整窗口大小时需要重新布局排列
    Q_UNUSED(event);
    createtable();
}


void showpdfForm::on_tableWidget_cellDoubleClicked(int row, int column)
{
    //双击cell事件
    //打开文件
    //    QMessageBox::information(this,"","test!!!!!");
    QString pdfpath = ui->tableWidget->item(row,column)->data(Qt::UserRole).toString();
    emit sentfilepath(pdfpath);//发送自定义信号给主窗口
}

