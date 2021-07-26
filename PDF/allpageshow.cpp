#include "allpageshow.h"
#include "ui_allpageshow.h"

AllPageShow::AllPageShow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AllPageShow)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);
    ui->listView->setViewMode(QListView::IconMode);

    ui->listView->setFlow(QListView::TopToBottom);//从上到下

    //设置图标大小
//    ui->listView->setIconSize();
    //设置间距
//    ui->listView->setSpacing(10);
    //设置自适应布局
//    ui->listView->setResizeMode(QListView::Adjust);
    //不能移动
    ui->listView->setMovement(QListView::Static);

//    ui->listView->setFont(QFont("微软雅黑"));
    //无焦点
    ui->listView->setFocusPolicy(Qt::NoFocus);
    //无边框
    ui->listView->setFrameShape(QFrame::NoFrame);
    //不可编辑
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //不可选中
    ui->listView->setSelectionMode(QAbstractItemView::NoSelection);

}

AllPageShow::~AllPageShow()
{
    delete ui;
}
