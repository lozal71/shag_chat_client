#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QTcpSocket>
#include "protocol_in.h"
#include "protocol_out.h"

enum setCodeCommand {Auth = 1};

class chatClient : public QObject
{
    Q_OBJECT
public:
    chatClient();
    ~chatClient();
    void connectClient();
    void prepareQueryAuth();
    void sessionClose();
    void setLogin(QString param);
    void setPass(QString param);
private:
    QTcpSocket * socket;
    protocolOut *out;
    protocolIn *in;
    struct clientData {
        QString login;
        QString pass;
        int id;
        QString name;
        QVariantMap mapRooms;
        QVariantMap mapMessages;
    } client;
    void readRespond();
    void sendQuery();
signals:
    void sessionClosed(QString sParam);
    void serverResponded(QString sParam);

};

#endif // CHATCLIENT_H
