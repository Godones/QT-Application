#include "mygraphics.h"
#include<QtDebug>
#include<QScrollBar>


Mygraphics::Mygraphics(QWidget *parent) : QGraphicsView(parent)
{

}

void Mygraphics::scrollContentsBy(int dx, int dy)
{
    //滚动条事件
    QGraphicsView::scrollContentsBy(dx,dy);
    emit updatePDF();
}
/*
void Mygraphics::wheelEvent(QWheelEvent *event)
{
    //鼠标滚轮事件
    qDebug("滚轮");
    QGraphicsView::wheelEvent(event);
}*/
