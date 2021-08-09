#ifndef MULPDFFORM_H
#define MULPDFFORM_H

#include <QEvent>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QScrollArea>
#include <QScrollBar>
#include <QWidget>
#include <mulpage.h>

namespace Ui {
class MulPDFForm;
}

class MulPDFForm : public QWidget {
    Q_OBJECT

public:
    explicit MulPDFForm(QWidget* parent = nullptr);
    ~MulPDFForm();

    void nextpage();
    void prepage();
    void zoomIn(); //放大
    void zoomOut(); //缩小
    void locatepage(int); //跳转
    void openPDF(QString& PDFpath);

protected:
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

private:
    Ui::MulPDFForm* ui;
    bool mouse_is_press; //鼠标被按下
    int y; //鼠标按下时的值

    MulPage* PDF;
    QScrollArea* area;
    QVBoxLayout* vlayout;
};

#endif // MULPDFFORM_H
