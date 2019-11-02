#ifndef PROTOCOLIN_H
#define POROTOCOLIN_H
#include <QDataStream>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonArray>

//enum setCodeCommand {Auth};

class protocolIn: public QObject
{

public:
    protocolIn();
    //protocolIn(QTcpSocket *socket);
//    setCodeCommand getCode();
//    QJsonObject getData();
     QJsonDocument receiveJSONdoc(QTcpSocket *socket);
    //QVariantMap getMAP();
     bool isError();
private:
//    setCodeCommand codeCommand;
//    QJsonDocument joDataInput;
    bool flag_error;
   // QJsonDocument getJSONdoc(QTcpSocket *socket);
};

#endif // POROTOCOLIN_H
