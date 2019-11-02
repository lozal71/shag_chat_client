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
    //protocolOut(QJsonDocument &jdTemp);
    QByteArray getPackage();
    void setPackage(QJsonDocument jdParam);
private:
    QByteArray baPackage;
//    QJsonDocument jdPackage;
};

#endif // PROTOCOLOUT_H
