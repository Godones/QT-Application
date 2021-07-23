#include "mainwindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QPainter>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->mdiArea);
    ui->mdiArea->setViewMode(QMdiArea::TabbedView);//多页显示
    ui->mdiArea->setTabsClosable(true);//打开关闭按钮
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openpdf(QString filepath)
{
    FormPdf * pdf = new FormPdf(this);//子页面
    ui->mdiArea->addSubWindow(pdf);
    //加载文件
    Poppler::Document *pdfdoc = Poppler::Document::load(filepath);

    QRect mrect;//获取设备分辨率
    mrect =QGuiApplication::primaryScreen()->geometry();
    qDebug()<<mrect.width()<<" "<<mrect.height();

    if(pdfdoc!=nullptr){
        pdfdoc->setRenderHint(Poppler::Document::TextAntialiasing);
        QSize size = pdfdoc->page(0)->pageSize();//页面大小
        qDebug()<<size;
        QImage image= pdfdoc->page(0)->renderToImage();
        pdf->loadpdf(image);
        pdf->show();

    }
    delete pdfdoc;
}

void MainWindow::showpdfslot(QString filepath)
{
    openpdf(filepath);
}

void MainWindow::isbelongQt(QString &dir)
{
    //判断是否是QT创建的仓库
}

void MainWindow::on_openfileaction_triggered()
{
    //打开仓库，需要识别是否是QT创建的仓库
    QString path = QFileDialog::getExistingDirectory(this);
    if(path.isEmpty())return;
    showpdfForm *showform = new showpdfForm;
    ui->mdiArea->addSubWindow(showform);
    showform->init(path);
    showform->show();
    connect(showform,&showpdfForm::sentfilepath,this,&MainWindow::showpdfslot);//连接子窗口与父窗口
}

void MainWindow::inputresposityname(QString& resposity)
{
    //输入仓库名称
    bool ok;
    QString title = "提示";
    QString inputtitle ="仓库名称" ;
    QInputDialog *input = new QInputDialog(this);
    input->setMinimumSize(300,400);

    QString text =input->getText(this,title,
    inputtitle, QLineEdit::Normal,
    "", &ok);
    if (ok && !text.isEmpty())
        resposity = text;
    delete input;
}

void MainWindow::on_createlibaction_triggered()
{
    //新建仓库
    //添加存储库
    //需要在指定目录下新建一个文件夹，之后再添加目录，即图片类别文件夹，之后的图片都是放在此文件夹下
    //只可以在图片类别文件下添加图片，且图片会被移动到此文件下。只可以在存储库下添加图片类别文件夹
    //添加组节点
    QString dir = QFileDialog::getExistingDirectory();//打开文件选择框，选择仓库目录
    QDir repo(dir);
    if(dir.isEmpty()) return;
    QString resposityname;
    inputresposityname(resposityname);//获取仓库名称
    if(!resposityname.isEmpty())
    {
        repo.mkdir(resposityname);//建立仓库
        QFile file(dir+"/+resposity.txt");
        file.open(QIODevice::ReadWrite|QIODevice::Text);
        file.write("QT CREATE\n");
    }

}


void MainWindow::on_typeaction_triggered()
{
    //新建分类

}


void MainWindow::on_openoneaction_triggered()
{
    //打开pdf文件
    QString filepath = QFileDialog::getOpenFileName(this,"","",tr("pdf(*.pdf)"));//打开pdf文件
    openpdf(filepath);
}

