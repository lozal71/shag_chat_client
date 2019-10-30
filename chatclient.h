#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QTcpSocket>
#include "protocol_in.h"
#include "protocol_out.h"

class chatClient : public QObject
{
    Q_OBJECT
public:
    chatClient();
public slots:
    void slotSendQuery(QJsonObject joParam);
    void slotSessionClose();
private:
    QTcpSocket * socket;
    QJsonObject joRespond;
    setCodeCommand codeCommand;
    void readRespond();

signals:
    void sessionClosed(setCodeCommand code, QJsonObject joRespond);
    void serverResponded(setCodeCommand code, QJsonObject joRrespond);

};

#endif // CHATCLIENT_H
