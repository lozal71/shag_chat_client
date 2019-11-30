#ifndef PROTOCOLOUT_H
#define PROTOCOLOUT_H
#include <QDataStream>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

class protocol_out: public QObject
{
public:
    protocol_out();
    QByteArray getPackage();
    void setPackage(QJsonDocument jdParam);
private:
    QByteArray baPackage;
};

#endif // PROTOCOLOUT_H
