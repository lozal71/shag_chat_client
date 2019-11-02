#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QVariantMap>
#include <QList>
#include  <stdarg.h>

class protocol
{
public:
    protocol();
    QJsonDocument getJsonDoc(QString *str, ...);
};

#endif // PROTOCOL_H
