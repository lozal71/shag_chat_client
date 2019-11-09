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
    QString sTemp;
    if (in->isError()){
        sTemp = "Problem: error massage";
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
                     sTemp = "Problem: login or password is not correct";
                }
                else {
                    sTemp = "Authorization is success:";
                    sTemp += " id=" + QString::number(client.id);
                    client.name = mapData["name"].toString();
                    sTemp += " name = " + client.name;
                    QVariantMap mapRooms = mapData["rooms"].toMap();
                    sTemp += "rooms: ";
                    for (const QString& roomID: mapRooms.keys()){
                        QVariantMap mapRoomID = mapRooms[roomID].toMap();
                        sTemp += roomID + "-";
                        for (const QString& roomName: mapRoomID.keys()){
                            sTemp +=roomName;
                            QVariantMap mapRoomName = mapRoomID[roomName].toMap();
                            for (const QString& timeMess: mapRoomName.keys()){
                                sTemp += timeMess + ": ";
                                QVariantMap mapSenderMessage = mapRoomName[timeMess].toMap();
                                for (const QString& sender: mapSenderMessage.keys()){
                                    sTemp += sender + " " + mapSenderMessage[sender].toString();
                                }
                            }
                        }
                    }
                    emit serverRespondedAuth(mapData["rooms"].toMap());
                }
                break;
            }
            case setCodeCommand::Send:
            {
                sTemp = "sendResult " + mapData["sendResult"].toString();
                //emit serverResponded();
                break;
            }
        }
    }
    qDebug() << sTemp;
    //emit serverRespondedLog(sTemp);
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
      // формирование JSON- документа
    QVariantMap mapCommand;
    QVariantMap mapData;
    /*QDateTime td;
    td = td.currentDateTime();*/
    mapData["roomID"] = roomID;
//    qDebug() << "client.id" << client.id;
//    mapData["id"] =client.id;
    mapData["text"] = client.text;
    //mapData["time"] = td;
    mapCommand["codeCommand"] = setCodeCommand::Send;
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
    emit sessionClosed("Disconnect. Session closed \n");
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

//QVariantMap chatClient::getMapMessage()
//{
//    return client.mapMessages;
//}


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
            emit sessionClosed("Problem: server time out \n");
            //qDebug() << "time out";
        }
    }
    else{
        //emit serverRespondedLog("Problem: no connection to server \n");
        qDebug() << "Problem: no connection to server";
    }
}

