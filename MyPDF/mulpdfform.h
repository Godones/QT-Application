#ifndef MULPDFFORM_H
#define MULPDFFORM_H

#include <QDockWidget>
#include <QEvent>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QScrollArea>
#include <QScrollBar>
#include <QSplitter>
#include <QTreeWidget>
#include <QWidget>
#include <mulpage.h>
#include <xmlload.h>

namespace Ui {
class MulPDFForm;
}

class MulPDFForm : public QWidget {
    Q_OBJECT

public:
    explicit MulPDFForm(QWidget* parent = nullptr);
    ~MulPDFForm();

    bool openPDF(QString PDFpath);

    int currentpage;
    qreal zoom;
    int numpages;
    QString PdfPath; //三个需要交互的变量

signals:
    void pagechanged(int currentpage, int totalpages, qreal zoom);

public slots:
    void nextpage();
    void prepage();
    void zoomIn(); //放大
    void zoomOut(); //缩小
    void fitpageshow();
    void fitwindowshow();
    void locatepage(int); //跳转

protected:
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

protected slots:
    void treeWidget_itemClicked(QTreeWidgetItem* item, int column);

private:
    Ui::MulPDFForm* ui;
    bool mouse_is_press; //鼠标被按下
    int y; //鼠标按下时的值
    int x;

    MulPage* PDF;
    QScrollArea* area;
    QVBoxLayout* vlayout;

    QSplitter* splitter;
    QDockWidget* dock;
    QTreeWidget* treeWidget;
    QHBoxLayout* hlayout;

    void updateinfo(int current, int totalpages, qreal zoom);
};

#endif // MULPDFFORM_H
