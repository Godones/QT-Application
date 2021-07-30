#include "mainwindow.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->tabWidget);

    //设置应用标题
    setWindowTitle("GodPDF");
    Locatedpage = new QSpinBox(this);

    Locatedpage->setMinimum(0);
    ui->toolBar->addSeparator();


    connect(Locatedpage,SIGNAL(valueChanged(int)),this,SLOT(Locatedpage_valchanged(int)));

    Scaling = new QSlider(Qt::Horizontal);
    //设置滑动条控件的最小值
    Scaling->setMinimum(20);
     //设置滑动条控件的最大值
     Scaling->setMaximum(300);
     //设置滑动条控件的值
    Scaling->setValue(100);
    Scaling->setMaximumWidth(100);
    Scaling->setTickPosition(QSlider::TicksBelow);
    Scaling->setTickInterval(25);
    Scaling->setSingleStep(25);

     //信号和槽 当滑动条的值发生改变时，即产生一个valueChanged(int)信号 设置QLineEdit控件的显示文本
    connect(Scaling, SIGNAL(valueChanged(int)), this, SLOT(resetScale(int)));

    ui->toolBar->addWidget(Locatedpage);

    pagenums = new QLabel();
    ui->toolBar->addSeparator();
    ui->toolBar->addWidget(pagenums);

    ui->toolBar->addSeparator();
    ui->toolBar->addWidget(Scaling);


    //Page有关闭按钮，可被关闭
    ui->tabWidget->clear();
    ui->tabWidget->setTabsClosable(true);

    //一开始将打开文件按钮设置不可使用

    ui->treeWidget->setWindowTitle("书签");

    //设置按钮可见性
    setActionTF();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setActionTF()
{
    ui->nextpageaction->setEnabled(false);
    ui->prepageaction->setEnabled(false);
    ui->addfileaction->setEnabled(false);
    ui->fitaction->setEnabled(false);
    ui->fitwindowsaction->setEnabled(false);
    ui->allpageaction->setEnabled(false);
    Locatedpage->setEnabled(false);
    Scaling->setEnabled(false);
    Locatedpage->setEnabled(false);
    pagenums->setEnabled(false);

    ui->dockWidget_2->setVisible(false);//初始界面dock栏不可见

}

void MainWindow::openpdf(QString filepath)
{
    //打开pdf并展示

    ui->treeWidget->clear();

    if(filepath.isEmpty())return;

    AllPageShow * pdf = new AllPageShow(this);//子页面

    pdf->setProperty("type","Mulpage");

    pdf->Pdfpath = filepath;

   if(pdf->load()){

       ui->treeWidget->clear();//清空标签
       get_xml_Marks(Poppler::Document::load(filepath));
       ui->treeWidget->expandAll();

       ui->dockWidget_2->setVisible(true);

       ui->tabWidget->addTab(pdf,getfinaldirname(filepath));

       ui->tabWidget->setCurrentWidget(pdf);

       pdf->Pdfpath = filepath;

       pagenums->setText(QString("%1").arg(pdf->numpages));

       Locatedpage->setMaximum(pdf->numpages-1);

   }
   else{
       QMessageBox::information(this,"提示","文件打开失败,可能已损坏!");
       delete  pdf;
   }

}

void MainWindow::showpdfslot(QString filepath)
{
    openpdf(filepath);
}

void MainWindow::isbelongQt(QString &dir)
{
    //判断是否是QT创建的仓库
    Q_UNUSED(dir);
}

void MainWindow::showtable(){
    //启用Qlistwidget方案
    PdfTable *showtablePDF = new PdfTable(this);
    showtablePDF->setProperty("type","List");
    ui->tabWidget->addTab(showtablePDF,getfinaldirname(Rootpath));
    ui->tabWidget->setCurrentWidget(showtablePDF);
    showtablePDF->init_read(Rootpath);
    showtablePDF->show();

    connect(showtablePDF,&PdfTable::sentfilepath,this,&MainWindow::showpdfslot);//连接子窗口与父窗口
    connect(this,&MainWindow::sentinformation_addfile,showtablePDF,&PdfTable::receive_information_addfile);

}

void MainWindow::on_openfileaction_triggered()
{
    //打开仓库，需要识别是否是QT创建的仓库
    //更新Rootpath
    Rootpath = QFileDialog::getExistingDirectory(this);
    if(Rootpath.isEmpty())return;
    showtable();
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
        showtable();//新建仓库的时候需要切换到当前仓库
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
    //适合页面显示
   QWidget *subwindow = ui->tabWidget->currentWidget();//获取活动窗口
   if(subwindow==nullptr)return;
   FormPdf *temp =(FormPdf*) subwindow;
   if(temp->property("type")=="page") temp->fitpageshow();

}

void MainWindow::on_fitwindowsaction_triggered()
{
    //适合窗口显示
    QWidget *subwindow = ui->tabWidget->currentWidget();//获取活动窗口
    if(subwindow==nullptr)return;
    FormPdf *temp =(FormPdf*) subwindow;
    if(temp->property("type")=="page") temp->fitwindowshow();
}

void MainWindow::on_nextpageaction_triggered()
{
    //下一页
    QWidget *subwindow = ui->tabWidget->currentWidget();//获取活动窗口
    FormPdf *temp =(FormPdf*) subwindow;
    if(temp->property("type")=="page") temp->nextview();
}


void MainWindow::on_prepageaction_triggered()
{
//    上一页
    QWidget *subwindow = ui->tabWidget->currentWidget();//获取活动窗口
    FormPdf *temp =(FormPdf*) subwindow;
    if(temp->property("type")=="page") temp->preview();
}

void MainWindow::Locatedpage_valchanged(int num)
{
    //定位到某一页
    qDebug()<<"LOin";

    QWidget *subwindow = ui->tabWidget->currentWidget();//获取活动窗口
    if(subwindow==nullptr)return;

    QString type = subwindow->property("type").toString();
    if(type=="page") {
        FormPdf *temp =(FormPdf*) subwindow;
        temp->located(num);
    }
    else if(type=="Mulpage"){
        AllPageShow * temp = (AllPageShow*)subwindow;
        temp->located(num);
    }

}

void MainWindow::resetScale(int scale)
{
    //QSlider滑动缩放
    QWidget *subwindow = ui->tabWidget->currentWidget();//获取活动窗口
    if(subwindow==nullptr)return;

    FormPdf *temp =(FormPdf*) subwindow;
    if(temp->property("type")=="page") temp->scale(scale);

    AllPageShow *show = static_cast<AllPageShow*>(subwindow);
    if(show->property("type")=="Mulpage"){
        show->setscale(scale);
    }

}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    //切换子窗口时发射
    //设置按钮可见性
    qDebug()<<"inT"<<ui->tabWidget->count();

    if(ui->tabWidget->count()==0){
        setActionTF();
        return;
    }
    QWidget *subwindow = ui->tabWidget->widget(index);//获取活动窗口
    if(subwindow==nullptr)return;
    QString type = subwindow->property("type").toString();
    if(type=="page"){
        FormPdf *lastpdf = static_cast<FormPdf*>(subwindow);
        ui->nextpageaction->setEnabled(true);
        ui->prepageaction->setEnabled(true);
        ui->addfileaction->setEnabled(false);
        ui->fitaction->setEnabled(true);
        ui->fitwindowsaction->setEnabled(true);
        ui->allpageaction->setEnabled(true);

        Locatedpage->setEnabled(true);
        Scaling->setEnabled(true);
        pagenums->setEnabled(true);
        Locatedpage->setMaximum(lastpdf->numpages-1);
        pagenums->setText(QString("%1").arg(lastpdf->numpages));

        ui->treeWidget->clear();
        get_xml_Marks(lastpdf->pdfdoc);
        ui->dockWidget_2->setVisible(true);
        ui->treeWidget->expandAll();

    }
    else if(type=="Mulpage"){
        AllPageShow *lastpdf = static_cast<AllPageShow*>(subwindow);
        ui->nextpageaction->setEnabled(false);
        ui->prepageaction->setEnabled(false);
        ui->allpageaction->setEnabled(true);
        ui->addfileaction->setEnabled(false);
        ui->fitaction->setEnabled(false);
        ui->fitwindowsaction->setEnabled(false);
        Locatedpage->setEnabled(true);
        Scaling->setEnabled(true);

        pagenums->setEnabled(true);

        Locatedpage->setMaximum(lastpdf->numpages-1);
        pagenums->setText(QString("%1").arg(lastpdf->numpages));

        ui->treeWidget->clear();
        get_xml_Marks(lastpdf->pdfdoc);
        ui->dockWidget_2->setVisible(true);
        ui->treeWidget->expandAll();
    }
    else {
        ui->nextpageaction->setEnabled(false);
        ui->prepageaction->setEnabled(false);
        ui->addfileaction->setEnabled(true);
        ui->fitaction->setEnabled(false);
        ui->fitwindowsaction->setEnabled(false);
        Locatedpage->setEnabled(false);
        Scaling->setEnabled(false);
        Locatedpage->setEnabled(false);
        pagenums->setEnabled(false);
        ui->dockWidget_2->setVisible(false);

    }

}

void MainWindow::on_allpageaction_triggered()
{
    //切换模式
    //单页还是多页显示
    qDebug("inA");


    QWidget *subwindow = ui->tabWidget->currentWidget();//获取活动窗口
    if(subwindow==nullptr)return;

    if(subwindow->property("type")=="Mulpage"){
        //当前是多页窗口，需要换到单页窗口
        FormPdf * pdf = new FormPdf(this);
        AllPageShow *lastpdf = static_cast<AllPageShow*>(subwindow);
        pdf->PdfPath = lastpdf->Pdfpath;
        pdf->loadpdf();
        pdf->located(lastpdf->currentpage);
        pdf->setProperty("type","page");
        int index = ui->tabWidget->currentIndex();
        ui->tabWidget->removeTab(index);
        QString namepdf = getfinaldirname(pdf->PdfPath);
        delete lastpdf;
        ui->tabWidget->insertTab(index,pdf,namepdf);
        ui->tabWidget->setCurrentWidget(pdf);


    }
    else{

        FormPdf *lastpdf = static_cast<FormPdf*>(subwindow);
        AllPageShow * pdf = new AllPageShow(this);//子页面
        pdf->setProperty("type","Mulpage");
        pdf->Pdfpath = lastpdf->PdfPath;//设置路径
        pdf->load();//加载
        pdf->located(lastpdf->currentpage);//定位

        int index = ui->tabWidget->currentIndex();
        ui->tabWidget->removeTab(index);
        delete lastpdf;//删除分页
        ui->tabWidget->insertTab(index,pdf,getfinaldirname(pdf->Pdfpath));//插入分页
        ui->tabWidget->setCurrentWidget(pdf);//显示当前页

    }


}
void MainWindow::get_xml_Marks(Poppler::Document *pdfdoc)
{

    QDomDocument* doc = pdfdoc->toc();
    if(doc==nullptr) return;

    ui->treeWidget->setColumnCount(1);
    ui->treeWidget->setWindowTitle("书签");
    ui->treeWidget->setSizeAdjustPolicy(QTreeWidget::AdjustToContents);
    //获取第一个节点
    qDebug()<<doc->toString();

    QDomNode node = doc->firstChild();
    while (!node.isNull()) {
        QDomElement e = node.toElement();
        QTreeWidgetItem *item  = new QTreeWidgetItem();
        item->setData(0,Qt::UserRole,e.attribute("Destination"));
        item->setText(0,e.tagName());
        ui->treeWidget->addTopLevelItem(item);
        if(node.hasChildNodes())
            read_xml(node.firstChild(),item);
        node = node.nextSibling();
    }
}

void MainWindow::read_xml(QDomNode node, QTreeWidgetItem * parent)
{

     // try to convert the node to an element.
    while (!node.isNull()) {
        QDomElement e = node.toElement();
        if (!e.isNull())
        {
//           qDebug() << e.tagName()<<e.attribute("Destination"); // the node really is an element.

           QTreeWidgetItem *item  = new QTreeWidgetItem();
           QString info = e.attribute(("Destination"));
           QString address="";
           for(int i=2;i<info.size();i++){
               if(info[i]==';')break;
               address +=info[i];
           }
           item->setData(0,Qt::UserRole,address.toInt());//存储位置

           item->setText(0,e.tagName());
           if(parent==nullptr)
                ui->treeWidget ->addTopLevelItem(item);
           else parent->addChild(item);

           if(e.hasChildNodes()) {
               //递归读取子节点
               read_xml(e.firstChild(),item);
           }
        }
        node = node.nextSibling();
    }
    return;
}




void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    //关闭页面
    ui->tabWidget->widget(index)->close();

}

void MainWindow::setLocatedpage(int index)
{
    //设置当前页面的值
    Locatedpage->setValue(index);
}


void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem *item, int column)
{
    //点击书签，跳转到指定位置
    int index = item->data(0,Qt::UserRole).toInt();
    QWidget * subwindow = ui->tabWidget->currentWidget();
    if(subwindow->property("type")=="page"){
        FormPdf *lastpdf = static_cast<FormPdf*>(subwindow);
        lastpdf->located(index);
    }
    else{
        AllPageShow * lastpdf  = static_cast<AllPageShow*>(subwindow);
        lastpdf->located(index);
    }
}

