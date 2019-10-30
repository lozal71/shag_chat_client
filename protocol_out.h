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
    protocolOut(QByteArray &str);
    QByteArray getMessageToClient();
private:
    QByteArray block;
};

#endif // PROTOCOLOUT_H
