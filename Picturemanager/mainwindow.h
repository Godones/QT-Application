#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QFileSystemModel>
#include<QLabel>
#include<QTreeView>
#include<QMimeDatabase>
#include<QFileDialog>
#include<QDialog>
#include<QMessageBox>
#include<QDebug>
#include<QInputDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected slots:
    void on_realsizeaction_triggered();
    void on_windowfloataction_triggered(bool checked);
private slots:
    void on_pictreeView_clicked(const QModelIndex &index);
    void on_fitwindowsaction_triggered();
    void on_fitheightaction_triggered();
    void on_bigeraction_triggered();
    void on_smalleraction_triggered();
    void on_fitwideaction_triggered();
    void on_openresposityaction_triggered();

    void on_Unfolddotaction_triggered();

    void on_deletedotaction_triggered();

    void on_windowlookaction_triggered(bool checked);
    void on_addrepositoryaction_triggered();

    void on_addsubsectionaction_triggered();

    void on_addfileaction_triggered();

private:
    Ui::MainWindow *ui;
    QFileSystemModel *model;

private:
    //创建节点时的类型，用作type参数
    //自定义的类型需要大于1000，因为内置的类型有一个为1000
    enum treeitemtype{topitem = 1001,grouptype,imagetype};
    //目录树的编号

    QLabel *filenamelabel;//工具栏显示文件路径
    QPixmap currentpic;//显示当前图片
    float picratio;//图片缩放指数

    //functions
    void addFolditem(QModelIndex &paritm,QString directory);//添加目录

    QString getfinaldirname(const QString&fulldirname);//获取目录名称

    void showpicitem(QModelIndex& item);//显示图片

    void addimageitem(QModelIndex &paritem,QString name);

    void self_connect();

    void unfold(QModelIndex &item);//展开目录

    void Waringmessage();//打印警告信息

    void inputresposityname(QString&resposityname);//输入仓库名称

    QString inputclasstypename();//输入分类文件夹名称

    bool createfolder(const QModelIndex& currentindex,const QString& dirname);//建立文件夹

    void deletefolder(QModelIndex &current);//删除文件夹及其里面的内容
};
#endif // MAINWINDOW_H
