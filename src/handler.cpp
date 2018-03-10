#include "handler.h"
Handler::Handler():QXmlDefaultHandler()
{

}
bool Handler::startElement(const QString & namespaceURI, const QString & localName, const QString & qName, const QXmlAttributes & atts)
{
    DataItem *item = new DataItem();
    if(localName==QStringLiteral("leaf")) {
        item->name= atts.value(QStringLiteral("name"));
        item->id = atts.value(QStringLiteral("id"));
        item->current=atts.value(QStringLiteral("current"));
        emit itemFound(item);
    }
    return true;
}
bool Handler::error(const QXmlParseException & exception)
{
qDebug() << exception.message() << exception.lineNumber();
}
bool Handler::endDocument()
{
emit mEndDocument();
    return true;
}
