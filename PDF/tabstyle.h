#ifndef TABSTYLE_H
#define TABSTYLE_H

#include <QProxyStyle>
#include <QPainter>
#include <QPainterPath>
#include<QColor>

class TabStyle : public QProxyStyle
{
public:
    TabStyle();
    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
                const QSize &size, const QWidget *widget) const;

    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const;

};

#endif // TABSTYLE_H
