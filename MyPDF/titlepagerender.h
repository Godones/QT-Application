#ifndef TITLEPAGERENDER_H
#define TITLEPAGERENDER_H

#include "poppler-qt5.h"
#include <QObject>
#include <QPixmap>
#include <QThread>
#include <QVector>

class TitlepageRender : public QThread {
    Q_OBJECT
public:
    explicit TitlepageRender(QObject* parent = nullptr);

    void set_data(QVector<QString>&, Priority = QThread::NormalPriority);

signals:
    void pagerendered(QPixmap, QString); //图片已经准备好

protected:
    void run();

private:
    void render();
    QVector<QString> Pdfpath;

    const qreal wideCover = 220; //展示封面大小
    const qreal heightCover = 300;
    const qreal wideGap = 10;
    const qreal heightGap = 80;
};

#endif // TITLEPAGERENDER_H
