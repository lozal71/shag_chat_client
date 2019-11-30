#include "chatclient.h"



chatClient::chatClient()
{
    socket = new QTcpSocket();
    out = new net(socket);
    //in = new protocolIn();
    client.id = 0;
    client.name.clear();
    connectClient();
}

chatClient::~chatClient()
{
    delete socket;
    delete out;
    //delete in;
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
    //QJsonDocument jdTemp = in->receiveJSONdoc(socket);
    //qDebug() << "35 jdTemp" << jdTemp;
    QVariantMap mapCommand = out->readSocket();
    QVariantMap mapData =  mapCommand["joData"].toMap();
    QString sLog;
    if (out->isError()){
        sLog = "Problem: error massage";
        qDebug() << sLog;
    }
    else{
        //QJsonObject joTemp = jdTemp.object();
        //qDebug() << "44 joTemp" << joTemp;
        //QVariantMap mapCommand =joTemp.toVariantMap();

        //qDebug() << "mapCommand" << mapCommand;
        //QVariantMap mapData =  mapCommand["joData"].toMap();
        //qDebug() << "mapData" << mapData;
        switch (setCodeCommand(mapCommand["codeCommand"].toInt())) {
        case setCodeCommand::auth:
        {
            //comm = Auth;
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
                emit authSuccess(mapData["rooms"].toMap());
                if (!mapData["invite"].toMap().isEmpty()){
                    emit readInvite(mapData["invite"].toMap());
                }
            }
            break;
        }
        case setCodeCommand::newMess:
        {
            //comm = newMess;
            sLog = "sendResult ";
            qDebug() << sLog;
            emit serverCast(mapData);
            break;
        }
        case setCodeCommand::newRoom:
        {
            //comm = NewRoom;
            sLog = "newRoomID " + mapData["newRoomID"].toString();
            sLog += "newRoomName" + mapData["newRoomName"].toString();
            qDebug() << sLog;
            emit serverRaspondedNewRoom(mapData);
            break;
        }
        case setCodeCommand::delRoom:
        {
            //comm = DelRoom;
            sLog = "delRoomID " + mapData["delRoomID"].toString();
            qDebug() << sLog;
            emit serverDeletedRoom(mapData["delRoomID"].toInt());
            break;
        }
        case setCodeCommand::messDelRoom:
        {
            //comm = MessDelRoom;
            sLog = "cast del room";
            qDebug() << sLog;
            //qDebug() << "mapData" << mapData;
            emit serverCastDelRoom(mapData);
            break;
        }
        case setCodeCommand::sendMess:
        {
            //comm = SendMess;
            sLog = "cast mess";
            qDebug() << sLog;
            //qDebug() << "91 mapData" << mapData;
            emit serverCast(mapData);
            break;
        }
        case setCodeCommand::invite:
        {
            //comm = Invite;
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
            //comm = acceptInvite;
            sLog = "accept invite";
            qDebug() << sLog;
            emit RoomsUserUpgrate(mapData);
            emit notifyUpgrated(mapData["inviteID"].toInt());
            break;
        }
        case setCodeCommand::questInvite:
        {
            //comm = questInvite;
            sLog = "quest invite";
            qDebug() << sLog;
            emit readInvite(mapData["invite"].toMap());
            break;
        }
        case setCodeCommand::rejectInvite:
        {
            //comm = rejectInvite;
            sLog = "reject invite";
            qDebug() << sLog;
            emit notifyUpgrated(mapData["inviteID"].toInt());
            break;
        }
        case setCodeCommand::delUser:
        {
            //comm = delUser;
            sLog = "delete user";
            qDebug() << sLog;
            emit serverCast(mapData);
            //mapData["updateParam"] = removeUser;
            emit usersUpdated(mapData);
            break;
        }
        case setCodeCommand::updateUsers:
        {
            //comm = updateUsers;
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
    mapCommand["codeCommand"] = setCodeCommand::auth;
    mapCommand["joData"] = mapData;
    //QJsonDocument jdQuery = QJsonDocument::fromVariant(mapCommand);
    //qDebug() << "jdQuery" << jdQuery;
    //comm = Auth;
    // сформировать выходной пакет для отправки на сервер

    //out->setPackage(jdQuery);
    // послать запрос
    qDebug() << "prepareQueryAuth";
    sendQuery(mapCommand);
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
    mapCommand["codeCommand"] = setCodeCommand::newMess;
    mapCommand["joData"] = mapData;
    //QJsonDocument jdQuery = QJsonDocument::fromVariant(mapCommand);
    //qDebug() << "jdQuery" << jdQuery;
    //comm = newMess;
    // сформировать выходной пакет для отправки на сервер
    //out->setPackage(jdQuery);
    // послать запрос
    qDebug() << "prepareQuerySendMessage";
    sendQuery(mapCommand);
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
    mapCommand["codeCommand"] = setCodeCommand::newRoom;
    mapCommand["joData"] = mapData;
    //QJsonDocument jdQuery = QJsonDocument::fromVariant(mapCommand);
    //qDebug() << "jdQuery" << jdQuery;
    //comm = NewRoom;
    // сформировать выходной пакет для отправки на сервер
    //out->setPackage(jdQuery);
    // послать запрос
    qDebug() << "prepareQueryNewRoom";
    sendQuery(mapCommand);
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
    mapCommand["codeCommand"] = setCodeCommand::delRoom;
    //comm = DelRoom;
    mapCommand["joData"] = mapData;
    //QJsonDocument jdQuery = QJsonDocument::fromVariant(mapCommand);
    //qDebug() << "jdQuery" << jdQuery;

    // сформировать выходной пакет для отправки на сервер
    //out->setPackage(jdQuery);
    // послать запрос
    qDebug() << "prepareQueryDelRoom";
    sendQuery(mapCommand);
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
      mapCommand["codeCommand"] = setCodeCommand::invite;
      //comm = Invite;
     /* mapCommand["joData"] = mapData;
      QJsonDocument jdQuery = QJsonDocument::fromVariant(mapCommand);*/
      //qDebug() << "jdQuery" << jdQuery;

      // сформировать выходной пакет для отправки на сервер
     // out->setPackage(jdQuery);
      // послать запрос
      qDebug() << "prepareQueryInvite";
      sendQuery(mapCommand);
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
      //comm = acceptInvite;
      mapCommand["joData"] = mapData;
//      QJsonDocument jdQuery = QJsonDocument::fromVariant(mapCommand);
//      //qDebug() << "jdQuery" << jdQuery;

//      // сформировать выходной пакет для отправки на сервер
//      out->setPackage(jdQuery);
      // послать запрос
      qDebug() << "prepareQueryAcceptInvite";
      sendQuery(mapCommand);
}

void chatClient::prepareQueryRejectInvite(int inviteID, int roomID, QString roomName,
                                          int senderID, QString senderName)
{
    if (socket->state() == QTcpSocket::UnconnectedState){
        socket->connectToHost("127.0.0.1", 6000);
    }
    // формирование JSON- документа
      QVariantMap mapCommand;
      QVariantMap mapData;
      mapData.insert("invitedName", client.name);
      mapData.insert("senderID", senderID);
      mapData.insert("senderName", senderName);
      mapData.insert("roomName", roomName);
      mapData.insert("roomID", roomID);
      mapData.insert("inviteID",inviteID);
      mapCommand["codeCommand"] = setCodeCommand::rejectInvite;
      mapCommand["joData"] = mapData;
//      QJsonDocument jdQuery = QJsonDocument::fromVariant(mapCommand);
//      //qDebug() << "jdQuery" << jdQuery;

//      // сформировать выходной пакет для отправки на сервер
//      out->setPackage(jdQuery);
      // послать запрос
      qDebug() << "prepareQueryRejectInvite";
      sendQuery(mapCommand);
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
      //comm = delUser;
      mapCommand["joData"] = mapData;
//      QJsonDocument jdQuery = QJsonDocument::fromVariant(mapCommand);
//      //qDebug() << "jdQuery" << jdQuery;

//      // сформировать выходной пакет для отправки на сервер
//      out->setPackage(jdQuery);
      // послать запрос
      qDebug() << "prepareQueryDelUser";
      sendQuery(mapCommand);
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

void chatClient::sendQuery(QVariantMap mapSocket)
{
    qDebug() << "sendQuery"; //<< comm;
    // если сокет дождался соединения с сервером
    if (socket->waitForConnected()){
         // запись выходного пакета в сокет
        //socket->write(out->writeSocket());
            qDebug() << "sendQuery";
        out->writeSocket(mapSocket);
        // ожидание ответа
        // если ответ не получен в течение 5 секунд
        if (!socket->waitForReadyRead(5000)) {
            // формируем ответ
            emit sessionClosed("Problem: server time out",
                               setDisconnect::fromServer);
            qDebug() << "time out";
        }
        else{
         qDebug() << " respond time in" ;//<< comm;
        }
    }
    else{
        qDebug() << "Problem: no connection to server";
        emit noConnect("Problem: no connection to server",
                       setDisconnect::fromServer);
    }
}

