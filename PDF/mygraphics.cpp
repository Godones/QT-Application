#include "mygraphics.h"
#include<QtDebug>
#include<QScrollBar>
#include<qmath.h>

Mygraphics::Mygraphics(QWidget *parent) : QGraphicsView(parent)
{

}

void Mygraphics::scrollContentsBy(int dx, int dy)
{
    //滚动条事件
//    qDebug()<<dx<<" "<<dy<<"!!!!";
    if(qAbs(dy)>=75){
        emit updatePDF();
    }
    QGraphicsView::scrollContentsBy(dx,dy);
}
/*
void Mygraphics::wheelEvent(QWheelEvent *event)
{
    //鼠标滚轮事件
    qDebug("滚轮");
    QGraphicsView::wheelEvent(event);
}*/
