#ifndef FORMPDF_H
#define FORMPDF_H

#include <QWidget>
#include<QImage>
#include<QLabel>
#include<QMessageBox>
#include<QPainter>


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
    void loadpdf(QImage& pdfimg);
    void loadpdf(QPainter *painter);


private:
    Ui::FormPdf *ui;

};

#endif // FORMPDF_H
