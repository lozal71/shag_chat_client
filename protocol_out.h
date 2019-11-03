#ifndef PROTOCOLOUT_H
#define PROTOCOLOUT_H
#include <QDataStream>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

class protocolOut: public QObject
{
public:
    protocolOut();
    QByteArray getPackage();
    void setPackage(QJsonDocument jdParam);
private:
    QByteArray baPackage;
};

#endif // PROTOCOLOUT_H
