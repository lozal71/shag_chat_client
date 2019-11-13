#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QTcpSocket>
#include "protocol_in.h"
#include "protocol_out.h"
#include <QDateTime>

enum setCodeCommand {Auth = 1, Send = 2, NewRoom = 3, DelRoom = 4, Cast = 5};

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
    void prepareQueryNewRoom(QString newRoomName);
    void prepareQueryDelRoom(int delRoomID);
    void sessionClose();
    void setLogin(QString param);
    void setPass(QString param);
    void setText(QString param);
    void setRoomActivName(QString param);
    QString getName();
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
    void serverRaspondedNewRoom(QVariantMap mapNewRoom);
    void serverCast(QVariantMap mapData);
    void serverDeletedRoom(int delRoomID);
};

#endif // CHATCLIENT_H
