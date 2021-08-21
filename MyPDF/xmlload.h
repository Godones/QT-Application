#ifndef XMLLOAD_H
#define XMLLOAD_H

#include <QString>
#include <QTreeWidget>
#include <poppler-qt5.h>
class XmlLoad {
public:
    XmlLoad(QString parh);
    void get_xml_Marks(QTreeWidget*);

    void read_xml(QDomNode node, QTreeWidget* treeWidget, QTreeWidgetItem* parent);

private:
    QString PdfPath;
};

#endif // XMLLOAD_H
