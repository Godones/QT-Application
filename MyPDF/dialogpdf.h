#ifndef DIALOGPDF_H
#define DIALOGPDF_H

#include <QDialog>

namespace Ui {
class DialogPDF;
}

class DialogPDF : public QDialog {
    Q_OBJECT

public:
    explicit DialogPDF(QWidget* parent = nullptr);
    ~DialogPDF();
    void load(QPixmap pix);

private:
    Ui::DialogPDF* ui;
};

#endif // DIALOGPDF_H
