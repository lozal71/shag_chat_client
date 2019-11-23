#include "chatclient.h"



chatClient::chatClient()
{
    socket = new QTcpSocket();
    out = new protocolOut();
    in = new protocolIn();
    client.id = 0;
    client.name.clear();
    connectClient();
}

chatClient::~chatClient()
{
    delete socket;
    delete out;
    delete in;
}

void chatClient::connectClient()
{
    // есть данные в сокете - читаем ответ сервера
    connect (socket, &QTcpSocket::readyRead,
             this, &chatClient::readRespond);
    // соединение прервалось - закрываем сессию
    connect(socket,&QTcpSocket::disconnected,
            this, &chatClient::sessionClose);
}

void chatClient::readRespond()
{
    // получаем JSON-документ из сокета
    QJsonDocument jdTemp = in->receiveJSONdoc(socket);
    //qDebug() << "35 jdTemp" << jdTemp;
    QString sLog;
    if (in->isError()){
        sLog = "Problem: error massage";
        qDebug() << sLog;
    }
    else{
        QJsonObject joTemp = jdTemp.object();
        //qDebug() << "44 joTemp" << joTemp;
        QVariantMap mapCommand =joTemp.toVariantMap();
        //qDebug() << "mapCommand" << mapCommand;
        QVariantMap mapData =  mapCommand["joDataInput"].toMap();
        //qDebug() << "mapData" << mapData;
        switch (setCodeCommand(mapCommand["codeCommand"].toInt())) {
        case setCodeCommand::Auth:
        {
            comm = Auth;
            sLog = "Auth";
            qDebug() << sLog;
            this->client.id = mapData["userID"].toInt();
            if (client.id ==0){
                 sLog = "Problem: login or password is not correct. Try again?";
                 emit authNotCorrected(sLog);
            }
            else {
                sLog = "Authorization is success:";
                //QVariantMap  mapUserName= mapData.first().toMap();
                this->client.id = mapData["userID"].toInt();
                this->client.name = mapData["userName"].toString();
                emit serverRespondedAuth(mapData["rooms"].toMap());
                if (!mapData["invite"].toMap().isEmpty()){
                    emit serverNotifyInvite(mapData["invite"].toMap());
                }
            }
            break;
        }
        case setCodeCommand::Send:
        {
            comm = Send;
            sLog = "sendResult ";
            qDebug() << sLog;
            emit serverCast(mapData);
            break;
        }
        case setCodeCommand::NewRoom:
        {
            comm = NewRoom;
            sLog = "newRoomID " + mapData["newRoomID"].toString();
            sLog += "newRoomName" + mapData["newRoomName"].toString();
            qDebug() << sLog;
            emit serverRaspondedNewRoom(mapData);
            break;
        }
        case setCodeCommand::DelRoom:
        {
            comm = DelRoom;
            sLog = "delRoomID " + mapData["delRoomID"].toString();
            qDebug() << sLog;
            emit serverDeletedRoom(mapData["delRoomID"].toInt());
            break;
        }
        case setCodeCommand::CastDelRoom:
        {
            comm = CastDelRoom;
            sLog = "cast del room";
            qDebug() << sLog;
            //qDebug() << "mapData" << mapData;
            emit serverCastDelRoom(mapData);
            break;
        }
        case setCodeCommand::CastMess:
        {
            comm = CastMess;
            sLog = "cast mess";
            qDebug() << sLog;
            //qDebug() << "91 mapData" << mapData;
            emit serverCast(mapData);
            break;
        }
        case setCodeCommand::Invite:
        {
            comm = Invite;
            if (mapData["invitedUserID"].toInt() == 0)
                sLog = "not exist user " + mapData["invitedUserName"].toString();
            else if (mapData["invitedUserID"].toInt() == -1)
                sLog = "user " + mapData["invitedUserName"].toString() + " уже участник этой комнаты";
            else {
                sLog = "to user " + mapData["invitedUserName"].toString() + " sended invite";
            }
            emit showResultInvite(sLog);
            qDebug() << sLog;
            break;
        }
        case setCodeCommand::acceptInvite:
        {
            comm = acceptInvite;
            sLog = "accept invite";
            qDebug() << sLog;
            emit RoomsUserUpgrate(mapData);
            emit notifyUpgrated(mapData["inviteID"].toInt());
            break;
        }
        case setCodeCommand::questInvite:
        {
            comm = questInvite;
            sLog = "quest invite";
            qDebug() << sLog;
            emit serverNotifyInvite(mapData["invite"].toMap());
            break;
        }
        case setCodeCommand::rejectInvite:
        {
            comm = rejectInvite;
            sLog = "reject invite";
            qDebug() << sLog;
            emit notifyUpgrated(mapData["inviteID"].toInt());
            break;
        }
        case setCodeCommand::delUser:
        {
            comm = delUser;
            sLog = "delete user";
            qDebug() << sLog;
            emit serverCast(mapData);
            mapData["updateParam"] = removeUser;
            emit usersUpdated(mapData);
            break;
        }
        case setCodeCommand::updateUsers:
        {
            comm = updateUsers;
            sLog = "updateUsers";
            qDebug() << sLog;
            emit usersUpdated(mapData);
            break;
        }
        }
    }

}

void chatClient::prepareQueryAuth(QString login, QString pass)
{
    if (socket->state() == QTcpSocket::UnconnectedState){
        socket->connectToHost("127.0.0.1", 6000);
    }
    // формирование JSON- документа
    QVariantMap mapCommand;
    QVariantMap mapData;
    mapData["login"] = login;
    mapData["pass"] = pass;
    mapCommand["codeCommand"] = setCodeCommand::Auth;
    mapCommand["joDataInput"] = mapData;
    QJsonDocument jdQuery = QJsonDocument::fromVariant(mapCommand);
    //qDebug() << "jdQuery" << jdQuery;
    comm = Auth;
    // сформировать выходной пакет для отправки на сервер
    out->setPackage(jdQuery);
    // послать запрос
    qDebug() << "prepareQueryAuth";
    sendQuery();
}

void chatClient::prepareQuerySendMessage(int roomID, QString text)
{
    if (socket->state() == QTcpSocket::UnconnectedState){
        socket->connectToHost("127.0.0.1", 6000);
    }
      // формирование JSON- документа
    QVariantMap mapCommand;
    QVariantMap mapData;
    mapData["roomID"] = roomID;
    mapData["text"] = text;
    mapCommand["codeCommand"] = setCodeCommand::Send;
    mapCommand["joDataInput"] = mapData;
    QJsonDocument jdQuery = QJsonDocument::fromVariant(mapCommand);
    //qDebug() << "jdQuery" << jdQuery;
    comm = Send;
    // сформировать выходной пакет для отправки на сервер
    out->setPackage(jdQuery);
    // послать запрос
    qDebug() << "prepareQuerySendMessage";
    sendQuery();
}

void chatClient::prepareQueryNewRoom(QString newRoomName)
{
    if (socket->state() == QTcpSocket::UnconnectedState){
        socket->connectToHost("127.0.0.1", 6000);
    }
      // формирование JSON- документа
    QVariantMap mapCommand;
    QVariantMap mapData;
    mapData["roomNew"] = newRoomName;
    mapCommand["codeCommand"] = setCodeCommand::NewRoom;
    mapCommand["joDataInput"] = mapData;
    QJsonDocument jdQuery = QJsonDocument::fromVariant(mapCommand);
    //qDebug() << "jdQuery" << jdQuery;
    comm = NewRoom;
    // сформировать выходной пакет для отправки на сервер
    out->setPackage(jdQuery);
    // послать запрос
    qDebug() << "prepareQueryNewRoom";
    sendQuery();
}

void chatClient::prepareQueryDelRoom(int delRoomID)
{
    if (socket->state() == QTcpSocket::UnconnectedState){
        socket->connectToHost("127.0.0.1", 6000);
    }
      // формирование JSON- документа
    QVariantMap mapCommand;
    QVariantMap mapData;
    mapData["delRoomID"] = delRoomID;
    mapCommand["codeCommand"] = setCodeCommand::DelRoom;
    comm = DelRoom;
    mapCommand["joDataInput"] = mapData;
    QJsonDocument jdQuery = QJsonDocument::fromVariant(mapCommand);
    //qDebug() << "jdQuery" << jdQuery;

    // сформировать выходной пакет для отправки на сервер
    out->setPackage(jdQuery);
    // послать запрос
    qDebug() << "prepareQueryDelRoom";
    sendQuery();
}

void chatClient::prepareQueryInvite(QString userName, QString text, int roomID)
{
    if (socket->state() == QTcpSocket::UnconnectedState){
        socket->connectToHost("127.0.0.1", 6000);
    }
    // формирование JSON- документа
      QVariantMap mapCommand;
      QVariantMap mapData;
      mapData["roomID"] = roomID;
      mapData["userName"] = userName;
      mapData["textInvite"] = text;
      mapCommand["codeCommand"] = setCodeCommand::Invite;
      comm = Invite;
      mapCommand["joDataInput"] = mapData;
      QJsonDocument jdQuery = QJsonDocument::fromVariant(mapCommand);
      //qDebug() << "jdQuery" << jdQuery;

      // сформировать выходной пакет для отправки на сервер
      out->setPackage(jdQuery);
      // послать запрос
      qDebug() << "prepareQueryInvite";
      sendQuery();
    }

void chatClient::prepareQueryAcceptInvite(int inviteID, int roomID, QString roomName)
{
    if (socket->state() == QTcpSocket::UnconnectedState){
        socket->connectToHost("127.0.0.1", 6000);
    }
    // формирование JSON- документа
      QVariantMap mapCommand;
      QVariantMap mapData;
      mapData["userName"] = client.name;
      mapData["roomName"] = roomName;
      mapData["roomID"] = roomID;
      mapData["inviteID"] = inviteID;
      mapCommand["codeCommand"] = setCodeCommand::acceptInvite;
      comm = acceptInvite;
      mapCommand["joDataInput"] = mapData;
      QJsonDocument jdQuery = QJsonDocument::fromVariant(mapCommand);
      //qDebug() << "jdQuery" << jdQuery;

      // сформировать выходной пакет для отправки на сервер
      out->setPackage(jdQuery);
      // послать запрос
      qDebug() << "prepareQueryAcceptInvite";
      sendQuery();
}

void chatClient::prepareQueryRejectInvite(int inviteID)
{
    if (socket->state() == QTcpSocket::UnconnectedState){
        socket->connectToHost("127.0.0.1", 6000);
    }
    // формирование JSON- документа
      QVariantMap mapCommand;
      QVariantMap mapData;
      mapData["inviteID"] = inviteID;
      mapCommand["codeCommand"] = setCodeCommand::rejectInvite;
      mapCommand["joDataInput"] = mapData;
      QJsonDocument jdQuery = QJsonDocument::fromVariant(mapCommand);
      //qDebug() << "jdQuery" << jdQuery;

      // сформировать выходной пакет для отправки на сервер
      out->setPackage(jdQuery);
      // послать запрос
      qDebug() << "prepareQueryRejectInvite";
      sendQuery();
}

void chatClient::prepareQueryDelUser(int userID, int roomID, QString text)
{
    if (socket->state() == QTcpSocket::UnconnectedState){
        socket->connectToHost("127.0.0.1", 6000);
    }
    // формирование JSON- документа
      QVariantMap mapCommand;
      QVariantMap mapData;
      mapData["roomID"] = roomID;
      mapData["userID"] = userID;
      mapData["text"] = text;
      mapCommand["codeCommand"] = setCodeCommand::delUser;
      comm = delUser;
      mapCommand["joDataInput"] = mapData;
      QJsonDocument jdQuery = QJsonDocument::fromVariant(mapCommand);
      //qDebug() << "jdQuery" << jdQuery;

      // сформировать выходной пакет для отправки на сервер
      out->setPackage(jdQuery);
      // послать запрос
      qDebug() << "prepareQueryDelUser";
      sendQuery();
}

void chatClient::sessionClose()
{
    emit sessionClosed("Disconnect. Session closed",
                       setDisconnect::undefined);
}

QString chatClient::getName()
{
    return client.name;
}

void chatClient::sendQuery()
{
    qDebug() << "sendQuery" << comm;
    // если сокет дождался соединения с сервером
    if (socket->waitForConnected()){
         // запись выходного пакета в сокет
        socket->write(out->getPackage());
        // ожидание ответа
        // если ответ не получен в течение 5 секунд
        if (!socket->waitForReadyRead(5000)) {
            // формируем ответ
            emit sessionClosed("Problem: server time out",
                               setDisconnect::fromServer);
            qDebug() << "time out";
        }
        else{
         qDebug() << " respond time in" << comm;
        }
    }
    else{
        qDebug() << "Problem: no connection to server";
        emit noConnect("Problem: no connection to server",
                       setDisconnect::fromServer);
    }
}

