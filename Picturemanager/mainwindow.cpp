#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("图片管理器");
    setCentralWidget(ui->scrollArea);
    model = new QFileSystemModel(this);
    ui->label->setScaledContents(true);
    ui->pictreeView->setModel(model);
    filenamelabel = new QLabel(this);
    ui->statusbar->addWidget(filenamelabel);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::self_connect()
{
    //将需要手动连接的信号进行连接
    connect(ui->pictreeView,SIGNAL(clicked(const QModelIndex &index)),this,SLOT(on_pictreeView_clicked(const QModelIndex &index)));

}



void MainWindow::on_pictreeView_clicked(const QModelIndex &index)
{
    //点击某个节点索引时触发
    filenamelabel->setText(model->filePath(index));
    QMimeDatabase db;
    QMimeType type = db.mimeTypeForFile(model->fileName(index));
    if(type.name().startsWith("image/"))
    {
        currentpic.load(model->filePath(index));//传入路径，载入图片
        on_fitwindowsaction_triggered();
    }
    else if(type.name().startsWith("text/")){
        QFile file(model->filePath(ui->pictreeView->currentIndex()));
        file.open(QIODevice::ReadOnly|QIODevice::Text);
        ui->label->clear();
        ui->label->setText(file.readAll());

    }
}

void MainWindow::on_fitwindowsaction_triggered()
{
    //图片以适合窗口显示
    int H = ui->scrollArea->height();
    int W = ui->scrollArea->width();
    int nowH = currentpic.height();
    int noww = currentpic.width();
    picratio = float(H)*W/nowH/noww;//设置缩放比例
    QPixmap newpic = currentpic.scaled(W,H);//返回一个缩放后的副本，其高度为H
    ui->label->setPixmap(newpic);//显示
}
void MainWindow::on_fitheightaction_triggered()
{
    //图片以适合高度显示
    int H = ui->scrollArea->height();
    int nowH = currentpic.height();
    picratio = float(H)/nowH;//设置缩放比例
    QPixmap newpic = currentpic.scaledToHeight(H);//返回一个缩放后的副本，其高度为H
    ui->label->setPixmap(newpic);//显示
}

void MainWindow::on_realsizeaction_triggered()
{
    //原图显示
    picratio = 1;
    ui->label->setPixmap(currentpic);
}

void MainWindow::on_fitwideaction_triggered()
{
    //图片以适合宽度显示
    int w = ui->scrollArea->width();
    int noww = currentpic.width();
    picratio = float(w)/noww;//设置缩放比例
    QPixmap newpic = currentpic.scaledToWidth(w);//返回一个缩放后的副本，其高度为H
    ui->label->setPixmap(newpic);//显示
}

void MainWindow::on_bigeraction_triggered()
{
    //放大
    picratio =1.2*picratio;
    int w = currentpic.width()*picratio;
    int h = currentpic.height()*picratio;
    QPixmap newpic = currentpic.scaled(w,h);
    ui->label->setPixmap(newpic);
}

void MainWindow::on_smalleraction_triggered()
{
    //缩小
    if(picratio>0.5)
        picratio = 0.8*picratio;
    int w = currentpic.width()*picratio;
    int h = currentpic.height()*picratio;
    QPixmap newpic = currentpic.scaled(w,h);
    ui->label->setPixmap(newpic);
}

void MainWindow::on_openresposityaction_triggered()
{
    //打开已有的仓库，需要判断是否是已经建立好的仓库，判断依据是文件夹下是否存在resposity.txt文件
    QString dir = QFileDialog::getExistingDirectory();//打开文件选择框，选择仓库目录
    if(QFile::exists(dir+"/resposity.txt"))//判断文件是否存在
    {
        model->setRootPath(dir);//这是设置model的根目录，即监视这个目录下的更改
        ui->pictreeView->setRootIndex(model->index(dir));//这是设置模型要显示目录的哪一部分
        //这样可以使得View里面显示的是仓库文件夹

    }
    else QMessageBox::warning(this,"Waring","请打开正确的仓库",QMessageBox::Ok);
}


void MainWindow::on_Unfolddotaction_triggered()
{
    //展开所有节点
//    for(int i=0;ui->pictreeView->topLevelWidget())
    ui->pictreeView->expandAll();
}

void MainWindow::on_windowfloataction_triggered(bool checked)
{
    //设置侧边栏浮动状态
    ui->dockWidget->setFloating(checked);
}

void MainWindow::on_windowlookaction_triggered(bool checked)
{
    //设置侧边栏可见性
    ui->dockWidget->setVisible(!checked);
    ui->windowfloataction->setEnabled(!checked);
}

void MainWindow::on_deletedotaction_triggered()
{
    //删除文件或文件夹
    QModelIndex currentindex = ui->pictreeView->currentIndex();
    QMessageBox::StandardButton recept;
    recept = QMessageBox::information(this,"提示","文件将被彻底删除",QMessageBox::Ok,QMessageBox::Cancel);
    if(recept==QMessageBox::Ok) model->remove(currentindex);
    else return;
}

void MainWindow::inputresposityname(QString& resposity)
{
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

QString MainWindow::inputclasstypename()
{
    //获取类别名称
    bool ok;
    QString title = "输入类别名称";
    QString showmessage = "类别名称";
    QString text = QInputDialog::getText(this,title,showmessage,QLineEdit::Normal,"",&ok);
    if(ok&&!text.isEmpty())
        return text;

    return "";
}
bool MainWindow::createfolder(const QModelIndex &folderindex, const QString &dirname)
{
    //新建文件夹
    model->mkdir(folderindex,dirname);
    return true;
}

void MainWindow::on_addrepositoryaction_triggered()
{

    //添加存储库
    //需要在指定目录下新建一个文件夹，之后再添加目录，即图片类别文件夹，之后的图片都是放在此文件夹下
    //只可以在图片类别文件下添加图片，且图片会被移动到此文件下。只可以在存储库下添加图片类别文件夹
    //添加组节点
    QString dir = QFileDialog::getExistingDirectory();//打开文件选择框，选择仓库目录
    if(dir.isEmpty()) return;
    QString resposityname;
    inputresposityname(resposityname);
    if(!resposityname.isEmpty()&&createfolder(model->index(dir),resposityname))
    {
        ui->addrepositoryaction->setEnabled(false);
        model->setRootPath(dir+"/"+resposityname);//重新设置模型根目录
        ui->pictreeView->setRootIndex(model->index(dir+"/"+resposityname));

        QFile file(model->rootPath()+"/resposity.txt");
        file.open(QIODevice::ReadWrite|QIODevice::Text);
        file.write("QT CREATE\n");
    }
}

void MainWindow::on_addsubsectionaction_triggered()
{
    //添加分类文件夹
    QString classtypename = inputclasstypename();
    if(!classtypename.isEmpty()){
        QModelIndex current = ui->pictreeView->currentIndex();
        if(model->isDir(current))//如果是一个目录
        {

            if(model->filePath(current)=="") model->mkdir(model->index(model->rootPath()),classtypename);
            else model->mkdir(current,classtypename);
        }
        else
        {
            model->mkdir(model->index(model->rootPath()),classtypename);
//            qDebug()<<model->filePath(current);
        }
    }
}

QString MainWindow::getfinaldirname(const QString &fulldirname)
{
    int length = fulldirname.length();
    int add = fulldirname.lastIndexOf("/");
    return fulldirname.right(length-add-1);
}
void MainWindow::on_addfileaction_triggered()
{
    //添加图片文件，会将图片移动到此此仓库下
    //过滤器
    QString filter = "images(*jpg *png *gif)";
    QStringList filenames = QFileDialog::getOpenFileNames(this,
                                                          "选择一个或多个文件","",filter);
    if(filenames.empty()) return;//没有选择文件就退出
    QModelIndex currentindex= ui->pictreeView->currentIndex();//获取当前所在节点
    if(currentindex.row()==-1) currentindex = model->index(model->rootPath());//设置为根目录
    if(!model->isDir(currentindex)) currentindex = model->parent(currentindex);//找到父节点
    for(auto x:filenames)
        QFile::rename(x,model->filePath(currentindex)+'/'+getfinaldirname(x));//移动文件
}


