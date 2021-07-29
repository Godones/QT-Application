#ifndef FORMPDF_H
#define FORMPDF_H

#include <QWidget>
#include<QImage>
#include<QLabel>
#include<QMessageBox>
#include<QPainter>
#include<poppler-qt5.h>
#include<QDebug>

namespace Ui {
class FormPdf;
}

class FormPdf : public QWidget
{
    Q_OBJECT

public:
    explicit FormPdf(QWidget *parent = nullptr);
    ~FormPdf();
public:
    QImage currentshow;

    QString PdfPath;//每个页面是一个pdf文件
    QString Pdfname;

    int currentpage;//当前页面

    Poppler::Document *pdfdoc ;
    bool loadpdf();

    void fitwindowshow();
    void fitpageshow();
    void scale(int factor);

    void nextview();
    void preview();
    void located(int num);

    int scaled;
private:

    Ui::FormPdf *ui;

};

#endif // FORMPDF_H
