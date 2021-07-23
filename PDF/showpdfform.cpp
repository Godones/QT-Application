#include "showpdfform.h"
#include "ui_showpdfform.h"


showpdfForm::showpdfForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::showpdfForm)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    connectSlot();
}

void showpdfForm::connectSlot()
{
    //连接信号
    connect(ui->tableWidget,SIGNAL(itemDoubleClicked(QTableWidgetItem *item)),this,SLOT(on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)));

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
        if(pdfdoc!=nullptr){
            QImage image= pdfdoc->page(0)->renderToImage();
            if(image.isNull()) continue;
            ico.push_back(image);
            name.push_back(dir.absoluteFilePath(allpdf[i]));
        }
    }
    createtable(ico,name);
}

void showpdfForm::createtable(QVector<QImage> &ico, QVector<QString> &names)
{
    //设置滚轮
    ui->tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    //设置行列总数，此处需要小心处理
    ui->tableWidget->setColumnCount(4);
    ui->tableWidget->setRowCount(4);

    int wide = this->width()/4;//获取table宽度
    for(int i=0;i<4;i++)
        ui->tableWidget->setColumnWidth(i,wide);
    for(int i=0;i<4;i++)
        ui->tableWidget->setRowHeight(i,wide*4/3);
    //设置行列标题的可见性
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setVisible(false);
    for(int i=0;i<ui->tableWidget->columnCount();i++){
        for(int j=0;j<ui->tableWidget->rowCount();j++){
            QLabel* label = new QLabel;
            label->setPixmap(QPixmap::fromImage(ico[i*4+j]).scaled(wide,wide*4/3));
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setData(Qt::UserRole,QVariant(name[i*4+j]));
            ui->tableWidget->setItem(i,j,item);
            ui->tableWidget->setCellWidget(i,j,label);
        }
    }

}

void showpdfForm::resizeEvent(QResizeEvent *event)
{
//    QMessageBox::information(this,"","waring");
    createtable(ico,name);
}




void showpdfForm::on_tableWidget_cellDoubleClicked(int row, int column)
{
    //双击cell事件
    //打开文件
    QMessageBox::information(this,"","test!!!!!");
    QString pdfpath = ui->tableWidget->item(row,column)->data(Qt::UserRole).toString();
    qDebug()<<pdfpath;
    emit sentfilepath(pdfpath);//发送自定义信号给主窗口
}

