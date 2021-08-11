#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->tabWidget);
    //设置应用标题
    setWindowTitle("GodPDF");

    inputpage = new QLineEdit(this); //输入页面
    allpages = new QLabel(this);

    inputpage->setMaximumWidth(50);

    connect(inputpage, &QLineEdit::returnPressed, this, &MainWindow::Locatedpage_valchanged); //回车跳转

    ui->toolBar->addSeparator();
    ui->toolBar->addWidget(inputpage);
    ui->toolBar->addWidget(allpages);

    zoomshow = new QLabel("zoom"); //显示缩放系数

    ui->toolBar->addSeparator();

    ui->toolBar->addAction(ui->bigaction);

    ui->toolBar->addWidget(zoomshow);

    ui->toolBar->addAction(ui->smalleraction);

    // Page有关闭按钮，可被关闭
    ui->tabWidget->clear();
    ui->tabWidget->setTabsClosable(true);

    ui->dockWidget_2->setWindowTitle("书签");
    ui->tabWidget->setMovable(true);

    //设置按钮可见性
    //    setActionTF();
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
    allpages->setEnabled(false);
    ui->bigaction->setEnabled(false);
    ui->smalleraction->setEnabled(false);

    inputpage->setEnabled(false);
    zoomshow->setEnabled(false);
    ui->dockWidget_2->setVisible(false); //初始界面dock栏不可见
}

void MainWindow::openpdf(QString filepath)
{
    //打开pdf并展示
    ui->treeWidget->clear();

    if (filepath.isEmpty())
        return;

    FormPdf* pdf = new FormPdf(this); //子页面
    pdf->setProperty("type", "page");
    pdf->PdfPath = filepath;
    if (pdf->loadpdf()) {
        ui->treeWidget->clear(); //清空标签
        get_xml_Marks(filepath); //加载标签
        //        ui->treeWidget->expandAll(); //展开所有标签
        //        if (ui->treeWidget->topLevelItemCount() == 0) {
        //            ui->dockWidget_2->setVisible(false);
        //        }
        ui->tabWidget->addTab(pdf, getfinaldirname(filepath));
        ui->tabWidget->setCurrentWidget(pdf);
        connect(pdf, &FormPdf::pagechanged, this, &MainWindow::updateinfo);

    } else {
        QMessageBox::information(this, "提示", "文件打开失败,可能已损坏!");
        delete pdf;
    }
}

void MainWindow::updateinfo(int index, int totalpages, qreal zoom)
{
    //更新当前页面值
    allpages->setText("/" + QString("%1").arg(totalpages - 1)); //总页面
    inputpage->setText(QString("%1").arg(index)); //当前页面
    inputpage->setMaximumWidth(50);
    zoomshow->setText(QString("%1").arg(int(zoom * 100)));
}

void MainWindow::showpdfslot(QString filepath)
{
    openpdf(filepath); //在table页面双击打开文件
}

void MainWindow::isbelongQt(QString& dir)
{
    //判断是否是QT创建的仓库
    Q_UNUSED(dir);
}

void MainWindow::showtable(QString dir)
{
    //启用Qlistwidget方案
    PdfTable* showtablePDF = new PdfTable(this);
    showtablePDF->setProperty("type", "List");

    ui->tabWidget->addTab(showtablePDF, getfinaldirname(dir));
    ui->tabWidget->setCurrentWidget(showtablePDF);
    showtablePDF->init_read(dir);
    showtablePDF->show();

    connect(showtablePDF, &PdfTable::sentfilepath, this,
        &MainWindow::showpdfslot); //连接子窗口与父窗口
    //    connect(this, &MainWindow::sentinformation_addfile, showtablePDF,
    //        &PdfTable::receive_information_addfile);
}

void MainWindow::on_openfileaction_triggered()
{
    //打开仓库，需要识别是否是QT创建的仓库
    //更新dir
    QString dir = QFileDialog::getExistingDirectory(this);
    if (dir.isEmpty())
        return;
    showtable(dir);
}

void MainWindow::inputresposityname(QString& resposity)
{
    //输入仓库名称
    bool ok;
    QString title = "提示";
    QString inputtitle = "仓库名称";
    QInputDialog* input = new QInputDialog(this);
    input->setMinimumSize(300, 400);

    QString text = input->getText(this, title, inputtitle, QLineEdit::Normal, "", &ok);
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
    QString dir = QFileDialog::getExistingDirectory(); //打开文件选择框，选择仓库目录
    QDir repo(dir);
    if (dir.isEmpty())
        return;
    QString resposityname;
    inputresposityname(resposityname); //获取仓库名称

    if (!resposityname.isEmpty()) {
        repo.mkdir(resposityname); //建立仓库
        QString filedir = dir + "/" + resposityname; //切换根目录
        //建立resposity文件存储一些信息
        QFile file(dir + "/" + resposityname + "/resposity.txt");
        file.open(QIODevice::ReadWrite | QIODevice::Text);
        file.write("QT CREATE\n");
        showtable(filedir); //新建仓库的时候需要切换到当前仓库
    }
}

void MainWindow::on_openoneaction_triggered()
{
    //打开pdf文件
    QString filepath = QFileDialog::getOpenFileName(
        this, "", "", tr("pdf(*.pdf)")); //打开pdf文件
    if (!filepath.isEmpty())
        openpdf(filepath);
}

QString MainWindow::getfinaldirname(const QString& fulldirname)
{
    //返回文件名
    int length = fulldirname.length();
    int add = fulldirname.lastIndexOf("/");
    return fulldirname.right(length - add - 1);
}

void MainWindow::on_addfileaction_triggered()
{
    //添加pdf文件
    //添加图片文件，会将图片移动到此此仓库下
    //过滤器

    QWidget* subwindow = ui->tabWidget->currentWidget(); //获取活动窗口
    PdfTable* table = static_cast<PdfTable*>(subwindow);

    QString filter = "pdf(*.pdf)";
    QStringList filenames = QFileDialog::getOpenFileNames(this, "选择一个或多个文件", "", filter);
    if (filenames.empty())
        return; //没有选择文件就退出
    QStringList newfilenames;
    for (auto& x : filenames) {
        QString newname = table->respdir + '/' + getfinaldirname(x);
        QFile::rename(x, newname); //移动文件
        newfilenames.append(newname);
    }

    table->receive_information_addfile(newfilenames); //直接调用子窗口添加
}

void MainWindow::on_fitaction_triggered()
{
    //适合页面显示
    QWidget* subwindow = ui->tabWidget->currentWidget(); //获取活动窗口
    if (subwindow == nullptr)
        return;
    if (subwindow->property("type") == "Mul") {
        MulPDFForm* temp = (MulPDFForm*)subwindow;
        temp->fitpageshow();
    }
    if (subwindow->property("type") == "page") {
        FormPdf* temp = (FormPdf*)subwindow;
        temp->fitpageshow();
    }
}

void MainWindow::on_fitwindowsaction_triggered()
{
    //适合窗口显示
    QWidget* subwindow = ui->tabWidget->currentWidget(); //获取活动窗口
    if (subwindow == nullptr)
        return;
    if (subwindow->property("type") == "Mul") {
        MulPDFForm* temp = (MulPDFForm*)subwindow;
        temp->fitwindowshow();
    }
    if (subwindow->property("type") == "page") {
        FormPdf* temp = (FormPdf*)subwindow;
        temp->fitwindowshow();
    }
}

void MainWindow::on_nextpageaction_triggered()
{
    //下一页
    QWidget* subwindow = ui->tabWidget->currentWidget(); //获取活动窗口
    if (subwindow == nullptr)
        return;

    if (subwindow->property("type") == "Mul") {
        MulPDFForm* temp = (MulPDFForm*)subwindow;
        temp->nextpage();
    }
    if (subwindow->property("type") == "page") {
        FormPdf* temp = (FormPdf*)subwindow;
        temp->nextpage();
    }
}

void MainWindow::on_prepageaction_triggered()
{
    //    上一页
    QWidget* subwindow = ui->tabWidget->currentWidget(); //获取活动窗口
    if (subwindow == nullptr)
        return;

    if (subwindow->property("type") == "Mul") {
        MulPDFForm* temp = (MulPDFForm*)subwindow;
        temp->prepage();
    }
    if (subwindow->property("type") == "page") {
        FormPdf* temp = (FormPdf*)subwindow;
        temp->prepage();
    }
}

void MainWindow::Locatedpage_valchanged()
{
    //定位到某一页
    //获取输入框的值
    bool ok;
    int num = inputpage->text().toInt(&ok);
    if (!ok)
        return; //如果转化成功

    QWidget* subwindow = ui->tabWidget->currentWidget(); //获取活动窗口
    if (subwindow == nullptr)
        return;
    if (subwindow->property("type") == "Mul") {
        MulPDFForm* temp = (MulPDFForm*)subwindow;
        temp->locatepage(num);
    }
    if (subwindow->property("type") == "page") {
        FormPdf* temp = (FormPdf*)subwindow;
        temp->locatepage(num);
    }
}
void MainWindow::on_bigaction_triggered()
{
    QWidget* subwindow = ui->tabWidget->currentWidget(); //获取活动窗口
    if (subwindow == nullptr)
        return;

    if (subwindow->property("type") == "Mul") {
        MulPDFForm* temp = (MulPDFForm*)subwindow;
        temp->zoomIn();
    }
    if (subwindow->property("type") == "page") {
        FormPdf* temp = (FormPdf*)subwindow;
        temp->zoomIn();
    }
}

void MainWindow::on_smalleraction_triggered()
{
    QWidget* subwindow = ui->tabWidget->currentWidget(); //获取活动窗口
    if (subwindow == nullptr)
        return;

    if (subwindow->property("type") == "Mul") {
        MulPDFForm* temp = (MulPDFForm*)subwindow;
        temp->zoomOut();
    }
    if (subwindow->property("type") == "page") {
        FormPdf* temp = (FormPdf*)subwindow;
        temp->zoomOut();
    }
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    //切换子窗口时发射
    //设置按钮可见性
    //    qDebug() << "inT" << ui->tabWidget->count();

    if (ui->tabWidget->count() == 0) {
        setActionTF();
        return;
    }
    QWidget* subwindow = ui->tabWidget->widget(index); //获取活动窗口
    if (subwindow == nullptr)
        return;
    QString type = subwindow->property("type").toString();
    if (type == "page" || type == "Mul") {
        QString file;
        int totalpages;
        int currentpage;
        qreal zoom;
        if (type == "page") {
            FormPdf* lastpdf = static_cast<FormPdf*>(subwindow);
            file = lastpdf->PdfPath;
            totalpages = lastpdf->numpages;
            currentpage = lastpdf->currentpage;
            zoom = lastpdf->m_zoom;
        } else {
            MulPDFForm* lastpdf = static_cast<MulPDFForm*>(subwindow);
            file = lastpdf->PdfPath;
            totalpages = lastpdf->numpages;
            currentpage = lastpdf->currentpage;
            zoom = lastpdf->zoom;
        }
        ui->nextpageaction->setEnabled(true);
        ui->prepageaction->setEnabled(true);
        ui->addfileaction->setEnabled(false);
        ui->fitaction->setEnabled(true);
        ui->fitwindowsaction->setEnabled(true);
        ui->allpageaction->setEnabled(true);
        allpages->setEnabled(true);
        inputpage->setEnabled(true);
        zoomshow->setEnabled(true);
        ui->bigaction->setEnabled(true);
        ui->smalleraction->setEnabled(true);

        ui->treeWidget->clear();
        get_xml_Marks(file);

        allpages->setText("/" + QString("%1").arg(totalpages - 1)); //总页面
        inputpage->setText(QString("%1").arg(currentpage)); //当前页面
        inputpage->setMaximumWidth(50);
        zoomshow->setText(QString("%1").arg(int(zoom * 100)));

        if (ui->treeWidget->topLevelItemCount() == 0) {
            ui->dockWidget_2->close();
        } else {
            ui->dockWidget_2->setVisible(true);
            ui->treeWidget->expandAll();
        }

    }

    else {
        ui->nextpageaction->setEnabled(false);
        ui->prepageaction->setEnabled(false);
        ui->addfileaction->setEnabled(true);
        ui->fitaction->setEnabled(false);
        ui->fitwindowsaction->setEnabled(false);
        ui->bigaction->setEnabled(false);
        ui->smalleraction->setEnabled(false);
        ui->allpageaction->setEnabled(false);
        allpages->setEnabled(false);
        inputpage->setEnabled(false);
        zoomshow->setEnabled(false);
        ui->dockWidget_2->setVisible(false);
    }
}

void MainWindow::on_allpageaction_triggered()
{
    //切换模式
    //单页还是多页显示

    QWidget* subwindow = ui->tabWidget->currentWidget(); //获取活动窗口
    if (subwindow == nullptr)
        return;

    if (subwindow->property("type") == "Mul") {
        //当前是多页窗口，需要换到单页窗口
        FormPdf* pdf = new FormPdf(this);
        MulPDFForm* lastpdf = static_cast<MulPDFForm*>(subwindow);
        pdf->PdfPath = lastpdf->PdfPath;
        pdf->loadpdf();
        pdf->locatepage(lastpdf->currentpage);
        pdf->setProperty("type", "page");
        int index = ui->tabWidget->currentIndex();
        ui->tabWidget->removeTab(index);
        QString namepdf = getfinaldirname(pdf->PdfPath);
        delete lastpdf;
        ui->tabWidget->insertTab(index, pdf, namepdf);
        ui->tabWidget->setCurrentWidget(pdf);

        //        Locatedpage->setMaximum(pdf->numpages - 1);
        //        Locatedpage->setSuffix("/" + (QString("%1").arg(pdf->numpages))); //设置后缀为总页数
        connect(pdf, &FormPdf::pagechanged, this, &MainWindow::updateinfo);

    } else {
        int current = 0;
        FormPdf* lastpdf = static_cast<FormPdf*>(subwindow);
        MulPDFForm* pdf = new MulPDFForm; //子页面
        pdf->setProperty("type", "Mul");
        pdf->PdfPath = lastpdf->PdfPath; //设置路径
        pdf->openPDF(pdf->PdfPath); //加载

        connect(pdf, &MulPDFForm::pagechanged, this, &MainWindow::updateinfo);
        current = lastpdf->currentpage;
        int index = ui->tabWidget->currentIndex();
        ui->tabWidget->removeTab(index);

        pdf->locatepage(lastpdf->currentpage); //定位
        delete lastpdf; //删除分页
        ui->tabWidget->insertTab(index, pdf, getfinaldirname(pdf->PdfPath)); //插入分页
        ui->tabWidget->setCurrentWidget(pdf); //显示当前页
        pdf->locatepage(current);
    }
}

void MainWindow::get_xml_Marks(QString PdfPath)
{
    Poppler::Document* pdfdoc = Poppler::Document::load(PdfPath);
    QDomDocument* doc = pdfdoc->toc();
    if (doc == nullptr)
        return;

    ui->treeWidget->setColumnCount(1);
    ui->treeWidget->setWindowTitle("书签");
    ui->treeWidget->setSizeAdjustPolicy(QTreeWidget::AdjustToContents);
    //获取第一个节点
    //    qDebug() << doc->toString();

    QDomNode node = doc->firstChild();
    while (!node.isNull()) {
        QDomElement e = node.toElement();
        QTreeWidgetItem* item = new QTreeWidgetItem();
        QString info = e.attribute(("Destination"));
        qDebug() << info;
        QStringList infolist = info.split(";");
        if (infolist.size() > 2)
            item->setData(0, Qt::UserRole, infolist.at(1).toInt() - 1);
        item->setText(0, e.tagName());
        ui->treeWidget->addTopLevelItem(item);
        if (node.hasChildNodes())
            read_xml(node.firstChild(), item);
        node = node.nextSibling();
    }
}

void MainWindow::read_xml(QDomNode node, QTreeWidgetItem* parent)
{
    // try to convert the node to an element.
    while (!node.isNull()) {
        QDomElement e = node.toElement();
        if (!e.isNull()) {
            //           qDebug() << e.tagName()<<e.attribute("Destination"); // the
            //           node really is an element.

            QTreeWidgetItem* item = new QTreeWidgetItem();
            QString info = e.attribute(("Destination"));
            QStringList infolist = info.split(";");
            if (infolist.size() > 2)

                item->setData(0, Qt::UserRole, infolist.at(1).toInt() - 1); //存储位置
            item->setText(0, e.tagName());
            if (parent == nullptr)
                ui->treeWidget->addTopLevelItem(item);
            else
                parent->addChild(item);

            if (e.hasChildNodes()) {
                //递归读取子节点
                read_xml(e.firstChild(), item);
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

void MainWindow::on_treeWidget_itemClicked(QTreeWidgetItem* item, int column)
{
    //点击书签，跳转到指定位置
    Q_UNUSED(column);
    int index = item->data(0, Qt::UserRole).toInt();
    QWidget* subwindow = ui->tabWidget->currentWidget();
    if (subwindow->property("type") == "page") {
        FormPdf* lastpdf = static_cast<FormPdf*>(subwindow);
        lastpdf->locatepage(index);
    } else {
        MulPDFForm* lastpdf = static_cast<MulPDFForm*>(subwindow);
        lastpdf->locatepage(index);
    }
}

void MainWindow::on_documentaction_triggered()
{
    QMessageBox::information(this, "应用介绍", "等等再写");
}
