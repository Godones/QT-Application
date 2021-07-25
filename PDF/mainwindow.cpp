#include "mainwindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->mdiArea);

    Locatedpage = new QSpinBox(this);
    Locatedpage->setMinimum(0);
    ui->toolBar->addSeparator();
    connect(Locatedpage,SIGNAL(valueChanged(int)),this,SLOT(on_Locatedpage_valchanged(int)));

    Scaling = new QSlider(Qt::Horizontal);
    //设置滑动条控件的最小值
    Scaling->setMinimum(0);
     //设置滑动条控件的最大值
     Scaling->setMaximum(200);
     //设置滑动条控件的值
    Scaling->setValue(100);
    Scaling->setMaximumWidth(100);
    Scaling->setTickPosition(QSlider::TicksBelow);
    Scaling->setTickInterval(25);
    Scaling->setSingleStep(25);

     //信号和槽 当滑动条的值发生改变时，即产生一个valueChanged(int)信号 设置QLineEdit控件的显示文本
    connect(Scaling, SIGNAL(valueChanged(int)), this, SLOT(resetScale(int)));


    ui->toolBar->addWidget(Locatedpage);
    ui->toolBar->addSeparator();
    ui->toolBar->addWidget(Scaling);
    ui->mdiArea->setViewMode(QMdiArea::TabbedView);//多页显示
    ui->mdiArea->setTabsClosable(true);//打开关闭按钮
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openpdf(QString filepath)
{
    //打开pdf并展示
    FormPdf * pdf = new FormPdf(ui->mdiArea);//子页面
    ui->mdiArea->addSubWindow(pdf);
    //加载文件
    pdf->PdfPath = filepath;
    pdf->Pdfname = getfinaldirname(filepath);
    pdf->loadpdf();
    pdf->show();

}

void MainWindow::showpdfslot(QString filepath)
{
    openpdf(filepath);
}

void MainWindow::isbelongQt(QString &dir)
{
    //判断是否是QT创建的仓库
}

void MainWindow::showpdftable()
{
    showpdfForm *showform = new showpdfForm(ui->mdiArea);
    ui->mdiArea->addSubWindow(showform);
    showform->init(Rootpath);
    showform->show();
    connect(showform,&showpdfForm::sentfilepath,this,&MainWindow::showpdfslot);//连接子窗口与父窗口
    connect(this,&MainWindow::sentinformation_addfile,showform,&showpdfForm::receive_information_addfile);
}

void MainWindow::on_openfileaction_triggered()
{
    //打开仓库，需要识别是否是QT创建的仓库
    //更新Rootpath
    Rootpath = QFileDialog::getExistingDirectory(this);
    if(Rootpath.isEmpty())return;
    showpdftable();
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
        Rootpath = dir +"/"+ resposityname;//切换根目录

        //建立resposity文件存储一些信息
        QFile file(dir+"/"+resposityname+"/resposity.txt");
        file.open(QIODevice::ReadWrite|QIODevice::Text);
        file.write("QT CREATE\n");
        showpdftable();//新建仓库的时候需要切换到当前仓库
    }

}

void MainWindow::on_openoneaction_triggered()
{
    //打开pdf文件
    QString filepath = QFileDialog::getOpenFileName(this,"","",tr("pdf(*.pdf)"));//打开pdf文件
    if(!filepath.isEmpty())
        openpdf(filepath);
}



QString MainWindow::getfinaldirname(const QString &fulldirname)
{
    //返回文件名
    int length = fulldirname.length();
    int add = fulldirname.lastIndexOf("/");
    return fulldirname.right(length-add-1);
}

void MainWindow::on_addfileaction_triggered()
{
    //添加pdf文件
    //添加图片文件，会将图片移动到此此仓库下
    //过滤器
    QString filter = "pdf(*.pdf)";
    QStringList filenames = QFileDialog::getOpenFileNames(this,
                                                          "选择一个或多个文件","",filter);
    if(filenames.empty()) return;//没有选择文件就退出

    QStringList newfilenames;
    for(auto &x:filenames){
        QString newname = Rootpath+'/'+getfinaldirname(x);
        QFile::rename(x,newname);//移动文件
        newfilenames.append(newname);
    }

    emit sentinformation_addfile(newfilenames);//发射信号给子窗口

}



void MainWindow::on_fitaction_triggered()
{
    //适合窗口显示
   QMdiSubWindow *subwindow = ui->mdiArea->activeSubWindow();//获取活动窗口
   FormPdf *temp =(FormPdf*) subwindow->widget();
   if(temp) temp->fitwindowshow();

}


void MainWindow::on_nextpageaction_triggered()
{
    //下一页
    QMdiSubWindow *subwindow = ui->mdiArea->activeSubWindow();//获取活动窗口
    FormPdf *temp =(FormPdf*) subwindow->widget();
    if(temp) temp->nextview();
}


void MainWindow::on_prepageaction_triggered()
{
//    上一页
    QMdiSubWindow *subwindow = ui->mdiArea->activeSubWindow();//获取活动窗口
    FormPdf *temp =(FormPdf*) subwindow->widget();
    if(temp) temp->preview();
}

void MainWindow::on_Locatedpage_valchanged(int num)
{
    //定位到某一页
    qDebug()<<num;
    QMdiSubWindow *subwindow = ui->mdiArea->activeSubWindow();//获取活动窗口
    FormPdf *temp =(FormPdf*) subwindow->widget();
    if(temp) temp->located(num-1);
}

void MainWindow::resetScale(int scale)
{
    //缩放
    qDebug()<<scale;
    QMdiSubWindow *subwindow = ui->mdiArea->activeSubWindow();//获取活动窗口
    FormPdf *temp =(FormPdf*) subwindow->widget();
    if(temp) temp->scale(scale);
}

