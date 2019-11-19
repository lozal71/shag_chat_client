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
    //qDebug() << "jdTemp" << jdTemp;
    QString sLog;
    if (in->isError()){
        sLog = "Problem: error massage";
    }
    else{
        QJsonObject joTemp = jdTemp.object();
        //qDebug() << "joTemp" << joTemp;
        QVariantMap mapCommand =joTemp.toVariantMap();
        //qDebug() << "mapCommand" << mapCommand;
        QVariantMap mapData =  mapCommand["joDataInput"].toMap();
        //qDebug() << "mapData" << mapData;
        switch (setCodeCommand(mapCommand["codeCommand"].toInt())) {
        case setCodeCommand::Auth:
        {
            this->client.id = mapData["userID"].toInt();
            if (client.id ==0){
                 sLog = "Problem: login or password is not correct. Try again?";
                 emit authNotCorrected(sLog);
            }
            else {
                sLog = "Authorization is success:";
                QVariantMap  mapUserName= mapData.first().toMap();
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
            sLog = "sendResult ";
            emit serverCast(mapData);
            break;
        }
        case setCodeCommand::NewRoom:
        {
            sLog = "newRoomID " + mapData["newRoomID"].toString();
            sLog += "newRoomName" + mapData["newRoomName"].toString();
            emit serverRaspondedNewRoom(mapData);
            break;
        }
        case setCodeCommand::DelRoom:
        {
            sLog = "delRoomID " + mapData["delRoomID"].toString();
            emit serverDeletedRoom(mapData["delRoomID"].toInt());
            break;
        }
        case setCodeCommand::CastDelRoom:
        {
            //qDebug() << "mapData" << mapData;
            emit serverCastDelRoom(mapData);
            break;
        }
        case setCodeCommand::CastMess:
        {
            //qDebug() << "91 mapData" << mapData;
            emit serverCast(mapData);
            break;
        }
        case setCodeCommand::Invite:
        {
            if (mapData["invitedUserID"].toInt() == 0)
                sLog = "not exist user " + mapData["invitedUserName"].toString();
            else {
                sLog = "to user " + mapData["invitedUserName"].toString() + " sended invite";
            }
            break;
        }
        case setCodeCommand::acceptInvite:
        {
            sLog = "accept invite";
            emit roomsUpgrated(mapData);
            emit notifyUpgrated(mapData["invitedID"].toInt());
            break;
        }
        case setCodeCommand::questInvite:
        {
            sLog = "quest invite";
            emit serverNotifyInvite(mapData["invite"].toMap());
            break;
        }
        }
    }
    qDebug() << sLog;
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

    // сформировать выходной пакет для отправки на сервер
    out->setPackage(jdQuery);
    // послать запрос
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

    // сформировать выходной пакет для отправки на сервер
    out->setPackage(jdQuery);
    // послать запрос
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

    // сформировать выходной пакет для отправки на сервер
    out->setPackage(jdQuery);
    // послать запрос
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
    mapCommand["joDataInput"] = mapData;
    QJsonDocument jdQuery = QJsonDocument::fromVariant(mapCommand);
    //qDebug() << "jdQuery" << jdQuery;

    // сформировать выходной пакет для отправки на сервер
    out->setPackage(jdQuery);
    // послать запрос
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
      mapCommand["joDataInput"] = mapData;
      QJsonDocument jdQuery = QJsonDocument::fromVariant(mapCommand);
      //qDebug() << "jdQuery" << jdQuery;

      // сформировать выходной пакет для отправки на сервер
      out->setPackage(jdQuery);
      // послать запрос
      sendQuery();
    }

void chatClient::prepareQueryAcceptInvite(int invitedID, int roomID, QString roomName)
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
      mapData["invitedID"] = invitedID;
      mapCommand["codeCommand"] = setCodeCommand::acceptInvite;
      mapCommand["joDataInput"] = mapData;
      QJsonDocument jdQuery = QJsonDocument::fromVariant(mapCommand);
      //qDebug() << "jdQuery" << jdQuery;

      // сформировать выходной пакет для отправки на сервер
      out->setPackage(jdQuery);
      // послать запрос
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
            //qDebug() << "time out";
        }
    }
    else{
        qDebug() << "Problem: no connection to server";
        emit noConnect("Problem: no connection to server",
                       setDisconnect::fromServer);
    }
}

