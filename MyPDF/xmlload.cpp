#include "xmlload.h"

XmlLoad::XmlLoad(QString path)
{
    PdfPath = path;
}

void XmlLoad::get_xml_Marks(QTreeWidget* treeWidget)
{
    Poppler::Document* pdfdoc = Poppler::Document::load(PdfPath);
    QDomDocument* doc = pdfdoc->toc();
    if (doc == nullptr)
        return;
    treeWidget->setColumnCount(1);
    treeWidget->setWindowTitle("书签");
    treeWidget->setSizeAdjustPolicy(QTreeWidget::AdjustToContents);
    //获取第一个节点
    //    qDebug() << doc->toString();

    QDomNode node = doc->firstChild();
    while (!node.isNull()) {
        QDomElement e = node.toElement();
        QTreeWidgetItem* item = new QTreeWidgetItem();
        QString info = e.attribute(("Destination"));
        //        qDebug() << info;
        QStringList infolist = info.split(";");
        if (infolist.size() > 2)
            item->setData(0, Qt::UserRole, infolist.at(1).toInt() - 1);
        item->setText(0, e.tagName());
        treeWidget->addTopLevelItem(item);
        if (node.hasChildNodes())
            read_xml(node.firstChild(), treeWidget, item);
        node = node.nextSibling();
    }
}

void XmlLoad::read_xml(QDomNode node, QTreeWidget* treeWidget, QTreeWidgetItem* parent)
{
    // try to convert the node to an element.
    while (!node.isNull()) {
        QDomElement e = node.toElement();
        if (!e.isNull()) {
            //           qDebug() << e.tagName()<<e.attribute("Destination"); // the
            //           node really is an element.

            QTreeWidgetItem* item = new QTreeWidgetItem();
            QString info = e.attribute(("Destination"));
            QStringList infolist = info.split(";");
            if (infolist.size() > 2)
                item->setData(0, Qt::UserRole, infolist.at(1).toInt() - 1); //存储位置
            item->setText(0, e.tagName());
            if (parent == nullptr)
                treeWidget->addTopLevelItem(item);
            else
                parent->addChild(item);

            if (e.hasChildNodes()) {
                //递归读取子节点
                read_xml(e.firstChild(), treeWidget, item);
            }
        }
        node = node.nextSibling();
    }
    return;
}
