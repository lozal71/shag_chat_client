#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QTcpSocket>
#include "protocol_in.h"
#include "protocol_out.h"
#include <QDateTime>

enum setCodeCommand {Auth = 1, Send = 2};

struct clientData {
    QString login;
    QString pass;
    int id;
    QString name;
    QString text;
};


class chatClient : public QObject
{
    Q_OBJECT
public:
    chatClient();
    ~chatClient();
    void connectClient();
    void prepareQueryAuth();
    void prepareQuerySendMessage(int roomID);
    void sessionClose();
    void setLogin(QString param);
    void setPass(QString param);
    void setText(QString param);
    QString getName();
//    QVariantMap getMapMessage();
private:
    QTcpSocket * socket;
    protocolOut *out;
    protocolIn *in;
    clientData client;
    void readRespond();
    void sendQuery();
signals:
    void sessionClosed(QString sParam);
    void serverRespondedAuth(QVariantMap mapRooms);

};

#endif // CHATCLIENT_H
