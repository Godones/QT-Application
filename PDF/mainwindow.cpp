#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->mdiArea);
    ui->mdiArea->setViewMode(QMdiArea::TabbedView);//多页显示
    ui->mdiArea->setTabsClosable(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_openfileaction_triggered()
{
    //打开pdf文件
    QString filepath = QFileDialog::getOpenFileName(this,"","",tr("pdf(*.pdf)"));//打开pdf文件
    if(filepath.isEmpty())return;

    FormPdf * pdf = new FormPdf(this);//子页面
    ui->mdiArea->addSubWindow(pdf);
    Poppler::Document *pdfdoc = Poppler::Document::load(filepath);
    if(pdfdoc!=nullptr){
        qDebug()<<"open";
        QImage image= pdfdoc->page(0)->renderToImage();
        pdf->loadpdf(image);
        pdf->show();
    }
    delete pdfdoc;
}

