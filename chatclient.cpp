#include "chatclient.h"


chatClient::chatClient()
{
    socket = new QTcpSocket();
    out = new protocolOut();
    loginClient.clear();
    passClient.clear();
    connect (socket, &QTcpSocket::readyRead,
             this, &chatClient::readRespond);
    connect(socket,&QTcpSocket::disconnected,
            this, &chatClient::slotSessionClose);
}

void chatClient::readRespond()
{

    protocolIn MessageIn(socket);
    joRespond = MessageIn.getData();
    codeCommand = MessageIn.getCode();
    emit serverResponded(codeCommand, joRespond);
}


void chatClient::prepareQueryAuth()
{
    if (socket->state() == QTcpSocket::UnconnectedState){
        socket->connectToHost("127.0.0.1", 6000);
    }
    // формирование JSON- документа
    QVariantMap mapCommand;
    QVariantMap mapData;
    mapData["login"] = loginClient;
    mapData["pass"] = passClient;
    mapCommand["codeCommand"] = setCodeCommand::Auth;
    mapCommand["joDataInput"] = mapData;
    QJsonDocument jdQuery = QJsonDocument::fromVariant(mapCommand);
    qDebug() << "jdQuery" << jdQuery;
    out->setPackage(jdQuery);
    sendQuery();
//    QByteArray baMessage = jdTemp.toJson(QJsonDocument::Compact);
    //qDebug() << message;
//    if (socket->waitForConnected()){
//        //qDebug() <<"We have connect to server!!! send message";
//        protocolOut MessageOut(baMessage);
//        socket->write(MessageOut.getMessageToClient());
//        if (!socket->waitForReadyRead(5000)) {
//            codeCommand = setCodeCommand::TimeOut;
//            QJsonObject joTemp;
//            joTemp.insert("codeCommand",setCodeCommand::TimeOut);
//            joTemp.insert("joDataInput","server timeout");
//            emit sessionClosed(TimeOut,joTemp);
//            qDebug() << "time out";
//        }
//        else{
//            qDebug() << "not time out";
//        }
//    }
//    else{
//        //qDebug() << "client say - no connect";
//        joRespond.insert("codeCommand",NoConnect);
//        joRespond.insert("joDataInput","no connection to server");
//        emit serverResponded(NoConnect,joRespond);
//    }
}

void chatClient::slotSessionClose()
{
    joRespond.insert("codeCommand",SessionClosed);
    joRespond.insert("joDataInput","session closed");
    emit sessionClosed(SessionClosed, joRespond);
}

void chatClient::setLogin(QString param)
{
    loginClient= param;
}

void chatClient::setPass(QString param)
{
    passClient = param;
}

void chatClient::sendQuery()
{
    if (socket->waitForConnected()){
        //qDebug() <<"We have connect to server!!! send message";
        //protocolOut MessageOut(baMessage);
        //socket->write(MessageOut.getMessageToClient());
//        qDebug() << "jdQuery" << jdQuery;
//        out->setPackage(jdQuery);
//         qDebug() << "out->getPackage()" << out->getPackage();
        socket->write(out->getPackage());
        if (!socket->waitForReadyRead(5000)) {
            codeCommand = setCodeCommand::TimeOut;
            QJsonObject joTemp;
            joTemp.insert("codeCommand",setCodeCommand::TimeOut);
            joTemp.insert("joDataInput","server timeout");
            emit sessionClosed(TimeOut,joTemp);
            qDebug() << "time out";
        }
        else{
            qDebug() << "not time out";
        }
    }
    else{
        //qDebug() << "client say - no connect";
        joRespond.insert("codeCommand",NoConnect);
        joRespond.insert("joDataInput","no connection to server");
        emit serverResponded(NoConnect,joRespond);
    }
}

