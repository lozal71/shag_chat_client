#include "chatclient.h"


chatClient::chatClient()
{
    socket = new QTcpSocket();
    out = new protocolOut();
    in = new protocolIn();
    client.id = 0;
    client.name.clear();
    client.pass.clear();
    client.login.clear();
    //client.mapRooms.clear();
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
                client.id = mapData["id"].toInt();
                if (client.id ==0){
                     sLog = "Problem: login or password is not correct";
                }
                else {
                    sLog = "Authorization is success:";
                    client.name = mapData["name"].toString();
                    //qDebug() << "mapData[\"rooms\"].toMap()" <<mapData["rooms"].toMap() ;
                    emit serverRespondedAuth(mapData["rooms"].toMap());
                }
                break;
            }
            case setCodeCommand::Send:
            {
                sLog = "sendResult " + mapData["sendResult"].toString();
                break;
            }
            case setCodeCommand::NewRoom:
            {
                sLog = "newRoomID " + mapData["newRoomID"].toString();
                sLog += "newRoomName" + mapData["newRoomName"].toString();
                emit serverRaspondedNewRoom(mapData);
                break;
            }
        }
    }
    qDebug() << sLog;
}


void chatClient::prepareQueryAuth()
{
    if (socket->state() == QTcpSocket::UnconnectedState){
        socket->connectToHost("127.0.0.1", 6000);
    }
    // формирование JSON- документа
    QVariantMap mapCommand;
    QVariantMap mapData;
    mapData["login"] = client.login;
    mapData["pass"] = client.pass;
    mapCommand["codeCommand"] = setCodeCommand::Auth;
    mapCommand["joDataInput"] = mapData;
    QJsonDocument jdQuery = QJsonDocument::fromVariant(mapCommand);
    //qDebug() << "jdQuery" << jdQuery;

    // сформировать выходной пакет для отправки на сервер
    out->setPackage(jdQuery);
    // послать запрос
    sendQuery();
}

void chatClient::prepareQuerySendMessage(int roomID)
{
    if (socket->state() == QTcpSocket::UnconnectedState){
        socket->connectToHost("127.0.0.1", 6000);
    }
      // формирование JSON- документа
    QVariantMap mapCommand;
    QVariantMap mapData;
    mapData["roomID"] = roomID;
    mapData["text"] = client.text;
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

void chatClient::sessionClose()
{
    emit sessionClosed("Disconnect. Session closed");
}

void chatClient::setLogin(QString param)
{
    client.login= param;
}

void chatClient::setPass(QString param)
{
    client.pass = param;
}

void chatClient::setText(QString param)
{
    client.text = param;
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
        //qDebug() << "out->getPackage()" << out->getPackage();
        socket->write(out->getPackage());
        // ожидание ответа
        // если ответ не получен в течение 5 секунд
        if (!socket->waitForReadyRead(5000)) {
            // формируем ответ
            emit sessionClosed("Problem: server time out \n");
            //qDebug() << "time out";
        }
    }
    else{
        //emit serverRespondedLog("Problem: no connection to server \n");
        qDebug() << "Problem: no connection to server";
    }
}

