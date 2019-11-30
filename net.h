#ifndef NET_H
#define NET_H
#include <QDataStream>
#include <QDebug>
#include <QJsonDocument>
#include <QTcpSocket>
#include <QJsonObject>

class net: public QObject
{
    Q_OBJECT
public:
    net();
    net(QTcpSocket *socket);
     void writeSocket(QVariantMap mapSocket);
    QVariantMap readSocket();
    bool isError();
private:
    QTcpSocket *socket;
    QByteArray baPackage;
    QJsonDocument jsonDoc;
    void transferJSONtoBA();
    void transferBAtoJSON();
    bool flag_error;
};

#endif // NET_H
