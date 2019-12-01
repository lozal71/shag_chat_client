#ifndef CHATCLIENT_H
#define CHATCLIENT_H

//#include <QTcpSocket>
#include "net.h"
//#include "protocol_in.h"
//#include "protocol_out.h"
#include <QDateTime>


enum setDisconnect {fromServer, fromClient, undefined};
enum setCodeCommand {auth = 1, newMess = 2, newRoom = 3, delRoom = 4,
                     messDelRoom = 5, sendMess = 6, invite = 7, questInvite = 8,
                     acceptInvite = 9, rejectInvite = 10, delUser = 11, updateUsers = 12};

struct clientData {
    int id;
    QString name;
};

//static setCodeCommand comm;

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
    void prepareQueryAcceptInvite(int inviteID, int roomID, QString roomName);
    void prepareQueryRejectInvite(int inviteID, int roomID, QString roomName,
                                  int senderID, QString senderName);
    void prepareQueryDelUser(int userID, int roomID, QString text);
    void sessionClose();
    void setRoomActivName(QString param);
    QString getName();
private:
    QTcpSocket * socket;
    net *protocol;
    clientData client;
    void readRespond();
    void sendQuery(QVariantMap mapSocket);
signals:
    void sessionClosed(QString sParam, setDisconnect discParam);
    void authSuccess(QVariantMap mapRooms);
    void usersUpdated(QVariantMap mapData);
    void serverRaspondedNewRoom(QVariantMap mapNewRoom);
    void serverCastDelRoom(QVariantMap mapData);
    void serverCast(QVariantMap mapData);
    void serverDeletedRoom(int delRoomID);
    void noConnect(QString sParam, setDisconnect discParam);
    void authNotCorrected(QString sParam);
    void readInvite(QVariantMap mapInvitations);
    void RoomsUserUpgrate(QVariantMap newRoom);
    void notifyUpgrated(int invitedID);
    void showResultInvite(QString sParam);
    void mapUsersReceived(QVariantMap mapUsers, int roomID);
};

#endif // CHATCLIENT_H
