#ifndef ALLPAGESHOW_H
#define ALLPAGESHOW_H

#include <QWidget>
#include<QListView>
namespace Ui {
class AllPageShow;
}

class AllPageShow : public QWidget
{
    Q_OBJECT

public:
    explicit AllPageShow(QWidget *parent = nullptr);
    ~AllPageShow();

private:
    Ui::AllPageShow *ui;
};

#endif // ALLPAGESHOW_H
