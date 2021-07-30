#include "tabstyle.h"
#include <QStyleOptionTab>


TabStyle::TabStyle()
{

}



QSize TabStyle::sizeFromContents(QStyle::ContentsType type, const QStyleOption *option, const QSize &size, const QWidget *widget) const
{
    QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
    if (type == QStyle::CT_TabBarTab) {
//        s.transpose();
        s.rwidth() = size.width(); // 设置每个tabBar中item的大小
        s.rheight() = 32;
    }
    return s;
}


void TabStyle::drawControl(QStyle::ControlElement element, const QStyleOption *option,
                                    QPainter *painter, const QWidget *w) const
{
    painter->save();
    painter->setPen(Qt::red);
//    painter->drawRect(w->rect());
    painter->restore();
    switch (element) {
    case CE_TabBarTabLabel:
    {
        const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option);
        QRect textRect = subElementRect(QStyle::SE_TabBarTabText, tab, w);

        if (tab->state & QStyle::State_Selected) {
            painter->save();
            painter->setPen(QPen("#000000"));
            QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
            option.setWrapMode(QTextOption::NoWrap);
            painter->drawText(textRect, tab->text, option);
            painter->restore();
        }
        else if(tab->state & QStyle::State_MouseOver) {
            painter->save();
            painter->setPen(QPen("#000000"));
            QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
            option.setWrapMode(QTextOption::NoWrap);
            painter->drawText(textRect, tab->text, option);
            painter->restore();
        }
        else
        {
            painter->save();
            painter->setPen(QPen("#666666"));
            QTextOption option(Qt::AlignLeft | Qt::AlignVCenter);
            option.setWrapMode(QTextOption::NoWrap);
            painter->drawText(textRect, tab->text, option);
            painter->restore();
        }
    }
        break;
    case CE_TabBarTabShape:
    {
        const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option);
        // Set Anti-aliasing effect
        painter->setRenderHint(QPainter::Antialiasing);

        QRectF shapeRect = tab->rect;
        qreal arcLen = shapeRect.height() / 2;

        if (tab->state & QStyle::State_Selected) {
            painter->save();
            QPen pen(Qt::red);
            pen.setWidthF(0.5);
            painter->setPen(pen);
//            painter->drawRect(shapeRect);
            painter->restore();

            painter->save();
            shapeRect.setX(shapeRect.x() - arcLen);
            shapeRect.setWidth(shapeRect.width() + arcLen *3/ 4);

            QPainterPath path;

            QRectF arcRect1(shapeRect.x(),
                            shapeRect.y() + shapeRect.height() / 3,
                            arcLen,
                            shapeRect.height() * 2 / 3);

            path.moveTo(QPointF(arcRect1.x(),
                                 arcRect1.y() + arcRect1.height()));

            path.lineTo(QPointF(arcRect1.x() + arcRect1.width() / 2,
                                 arcRect1.y() + arcRect1.height()));

            // Starting from 270°, returning to the arc of 90°
            path.arcTo(arcRect1, 270.0, 90.0);

            path.lineTo(QPointF(arcRect1.x() + arcRect1.width(),
                                 arcRect1.y()));

            QRectF arcRect2(shapeRect.x() + arcRect1.width(),
                            shapeRect.y(),
                            arcLen,
                            shapeRect.height() * 2 / 3);

            // Starting from 180°, returning to the arc of -90°
            path.arcTo(arcRect2, 180.0, -90.0);

            path.lineTo(QPointF(arcRect2.x() + shapeRect.width() -  2 * (arcRect2.x()  + arcRect2.width()) + arcRect2.width() / 2,
                                 arcRect2.y()));

            QRectF arcRect3(shapeRect.x() + shapeRect.width() - 2 * arcRect1.width(),
                            shapeRect.y(),
                            arcRect1.width(),
                            shapeRect.height() * 2 / 3);

            path.arcTo(arcRect3, 90.0, -90.0);

            path.lineTo(QPointF(arcRect3.x() + arcRect3.width(),
                                 arcRect3.y() + arcRect3.height()));

            QRectF arcRect4(arcRect3.x() + arcRect3.width(),
                            arcRect3.y() + arcRect3.height() / 2,
                            arcRect1.width(),
                            shapeRect.height() * 2 / 3);

            path.arcTo(arcRect4, 180.0, 90.0);

            path.lineTo(QPointF(arcRect1.x(),
                                arcRect1.y() + arcRect1.height()));

            painter->setPen(Qt::NoPen);
            painter->setBrush(Qt::white);
            QPolygonF polygon = path.toFillPolygon();
            painter->drawPolygon(polygon);

            painter->restore();
        }else if(tab->state & QStyle::State_MouseOver)
        {
//            return;
            painter->save();
            QPen pen(Qt::black);
            pen.setWidthF(0.5);
            painter->setPen(pen);
//            painter->drawRect(shapeRect);
            painter->restore();
            painter->save();

            shapeRect.setX(shapeRect.x() - arcLen);
            shapeRect.setWidth(shapeRect.width() + arcLen *3/ 4);

            QPainterPath path;

            QRectF arcRect1(shapeRect.x(),
                            shapeRect.y() + shapeRect.height() / 3,
                            arcLen,
                            shapeRect.height() * 2 / 3);

            path.moveTo(QPointF(arcRect1.x(),
                                 arcRect1.y() + arcRect1.height()));

            path.lineTo(QPointF(arcRect1.x() + arcRect1.width() / 2,
                                 arcRect1.y() + arcRect1.height()));

            // Starting from 270°, returning to the arc of 90°
            path.arcTo(arcRect1, 270.0, 90.0);

            path.lineTo(QPointF(arcRect1.x() + arcRect1.width(),
                                 arcRect1.y()));

            QRectF arcRect2(shapeRect.x() + arcRect1.width(),
                            shapeRect.y(),
                            arcLen,
                            shapeRect.height() * 2 / 3);

            // Starting from 180°, returning to the arc of -90°
            path.arcTo(arcRect2, 180.0, -90.0);

            path.lineTo(QPointF(arcRect2.x() + shapeRect.width() -  2 * (arcRect2.x()  + arcRect2.width()) + arcRect2.width() / 2,
                                 arcRect2.y()));

            QRectF arcRect3(shapeRect.x() + shapeRect.width() - 2 * arcRect1.width(),
                            shapeRect.y(),
                            arcRect1.width(),
                            shapeRect.height() * 2 / 3);

            path.arcTo(arcRect3, 90.0, -90.0);

            path.lineTo(QPointF(arcRect3.x() + arcRect3.width(),
                                 arcRect3.y() + arcRect3.height()));

            QRectF arcRect4(arcRect3.x() + arcRect3.width(),
                            arcRect3.y() + arcRect3.height() / 2,
                            arcRect1.width(),
                            shapeRect.height() * 2 / 3);

            path.arcTo(arcRect4, 180.0, 90.0);

            path.lineTo(QPointF(arcRect1.x(),
                                arcRect1.y() + arcRect1.height()));

            painter->setPen(Qt::NoPen);
            painter->setBrush(QColor("#FBFCFC"));
            QPolygonF polygon = path.toFillPolygon();
            painter->drawPolygon(polygon);

            painter->restore();

        }else
        {
            painter->save();

            shapeRect.setX(shapeRect.x() - arcLen);
            shapeRect.setWidth(shapeRect.width() + arcLen / 2);

            QPen pen(Qt::black);
            pen.setWidthF(0.5);
            painter->setPen(pen);

            /// Draw a dividing line on the left
            painter->drawLine(QPointF(shapeRect.x() + arcLen*2/3 -1, shapeRect.height() / 3),
                              QPointF(shapeRect.x() + arcLen*2/3 -1, shapeRect.height() * 2 / 3));

            /// Draw a dividing line on the right
//            painter->drawLine(baseR.x() + baseR.width() - arcLen,
//                              baseR.height() / 4,
//                              baseR.x() + baseR.width() - arcLen,
//                              baseR.height() * 3 / 4);

            painter->restore();
        }
    }
        break;
    default:
        QProxyStyle::drawControl(element, option, painter, w);
        break;
    }
}
