#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QTcpSocket>
#include "protocol_in.h"
#include "protocol_out.h"
#include <QDateTime>

enum setDisconnect {fromServer, fromClient, undefined};
enum setCodeCommand {Auth = 1, Send = 2, NewRoom = 3, DelRoom = 4,
                     CastDelRoom = 5, CastMess = 6, Invite = 7};

struct clientData {
    int id;
    QString name;
};


class chatClient : public QObject
{
    Q_OBJECT
public:
    chatClient();
    ~chatClient();
    void connectClient();
    void prepareQueryAuth(QString login, QString pass);
    void prepareQuerySendMessage(int roomID, QString text);
    void prepareQueryNewRoom(QString newRoomName);
    void prepareQueryDelRoom(int delRoomID);
    void prepareQueryInvite(QString userName, QString text, int roomID);
    void sessionClose();
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
    void sessionClosed(QString sParam, setDisconnect discParam);
    void serverRespondedAuth(QVariantMap mapRooms);
    void serverRaspondedNewRoom(QVariantMap mapNewRoom);
    void serverCastDelRoom(QVariantMap mapData);
    void serverCast(QVariantMap mapData);
    void serverDeletedRoom(int delRoomID);
    void noConnect(QString sParam, setDisconnect discParam);
    void authNotCorrected(QString sParam);
};

#endif // CHATCLIENT_H
