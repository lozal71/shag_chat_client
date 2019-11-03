#ifndef PROTOCOLIN_H
#define POROTOCOLIN_H
#include <QDataStream>
#include <QTcpSocket>
#include <QJsonDocument>

//enum setCodeCommand {Auth};

class protocolIn: public QObject
{

public:
    protocolIn();
    QJsonDocument receiveJSONdoc(QTcpSocket *socket);
    bool isError();
private:
    bool flag_error;
};

#endif // POROTOCOLIN_H
