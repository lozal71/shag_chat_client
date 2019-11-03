#include "chatclient.h"


chatClient::chatClient()
{
    socket = new QTcpSocket();
    out = new protocolOut();
    in = new protocolIn();
    data.idClient = 0;
    data.nameClient.clear();
    data.passClient.clear();
    data.loginClient.clear();
    data.mapRoomsClient.clear();
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
        data.idClient = mapData["id"].toInt();
        if (data.idClient ==0){
             sTemp = "Problem: login or rassword is not correct";
        }
        else {
            sTemp = "Authorization is success:";
            sTemp += " id=" + QString::number(data.idClient);
            data.nameClient = mapData["name"].toString();
            sTemp += " name = " + data.nameClient +"\n";
            data.mapRoomsClient = mapData["rooms"].toMap();
            sTemp += " rooms:";
            for (const QString& roomID: data.mapRoomsClient.keys()){
                sTemp += roomID + " - " + data.mapRoomsClient[roomID].toString() + "\n";
            }
        }
    }
    emit serverResponded(sTemp);
}


void chatClient::prepareQueryAuth()
{
    if (socket->state() == QTcpSocket::UnconnectedState){
        socket->connectToHost("127.0.0.1", 6000);
    }
    // формирование JSON- документа
    QVariantMap mapCommand;
    QVariantMap mapData;
    mapData["login"] = data.loginClient;
    mapData["pass"] = data.passClient;
    mapCommand["codeCommand"] = setCodeCommand::Auth;
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
    data.loginClient= param;
}

void chatClient::setPass(QString param)
{
    data.passClient = param;
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
            emit sessionClosed("Problem: server time out \n");
            //qDebug() << "time out";
        }
    }
    else{
        emit serverResponded("Problem: no connection to server \n");
    }
}

