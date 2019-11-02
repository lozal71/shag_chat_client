#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QTcpSocket>
#include "protocol_in.h"
#include "protocol_out.h"

enum setKeysJSON {codecommand, jData};

class chatClient : public QObject
{
    Q_OBJECT
public:
    chatClient();    
    void prepareQueryAuth();
    void slotSessionClose();
    void setLogin(QString param);
    void setPass(QString param);
private:
    QTcpSocket * socket;
    QJsonObject joRespond;
    QJsonDocument jdQuery;
    setCodeCommand codeCommand;
    protocolOut *out;
    QString loginClient;
    QString passClient;
    void readRespond();
    void sendQuery();
signals:
    void sessionClosed(setCodeCommand code, QJsonObject joRespond);
    void serverResponded(setCodeCommand code, QJsonObject joRrespond);

};

#endif // CHATCLIENT_H
