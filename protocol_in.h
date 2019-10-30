#ifndef PROTOCOLIN_H
#define POROTOCOLIN_H
#include <QDataStream>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>


enum setCodeCommand {ErrorMessage, Auth, SessionClosed, NoConnect, TimeOut};

class protocolIn: public QObject
{

public:
    //protocolIn();
    protocolIn(QTcpSocket *socket);
    setCodeCommand getCode();
    QJsonObject getData();
private:
    setCodeCommand codeCommand;
    QJsonObject joDataInput;
    QJsonObject getJsonObjectIN(QTcpSocket *socket);
};

#endif // POROTOCOLIN_H
