#ifndef MYGRAPHICS_H
#define MYGRAPHICS_H

#include <QGraphicsView>
#include<QWidget>
class Mygraphics : public QGraphicsView
{
    Q_OBJECT
public:
    explicit Mygraphics(QWidget *parent = nullptr);

signals:
    void updatePDF();
protected:
    void scrollContentsBy(int dx, int dy);
//    void wheelEvent(QWheelEvent *event);


};

#endif // MYGRAPHICS_H
