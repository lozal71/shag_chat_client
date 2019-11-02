#include "chatclient.h"


chatClient::chatClient()
{
    socket = new QTcpSocket();
    out = new protocolOut();
    in = new protocolIn();
    loginClient.clear();
    passClient.clear();
    connectClient();

}

chatClient::~chatClient()
{

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
    //in->receiveJSONdoc(socket);
//    protocolIn MessageIn(socket);
//    joRespond = MessageIn.getData();
//    codeCommand = MessageIn.getCode();
    QString sTemp;
    QJsonDocument jdTemp = in->receiveJSONdoc(socket);
    qDebug() << "jdTemp" << jdTemp;
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
        idClient = mapData["id"].toInt();
        if (idClient ==0){
             sTemp = "Problem: login or rassword is not correct";
        }
        else {
            sTemp = "Authorization is success:";
            sTemp += " id=" + QString::number(idClient);
            nameClient = mapData["name"].toString();
            sTemp += " name = " + nameClient +"\n";
            mapRoomsClient = mapData["rooms"].toMap();
            sTemp += " rooms:";
            for (const QString& roomID: mapRoomsClient.keys()){
                sTemp += roomID + " - " + mapRoomsClient[roomID].toString() + "\n";
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
    mapData["login"] = loginClient;
    mapData["pass"] = passClient;
    mapCommand["codeCommand"] = setCodeCommand::Auth;
    mapCommand["joDataInput"] = mapData;
    QJsonDocument jdQuery = QJsonDocument::fromVariant(mapCommand);
    //qDebug() << "jdQuery" << jdQuery;

    // сформировать выходной пакет для отправки на сервер
    out->setPackage(jdQuery);
    // послать запрос
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

void chatClient::sessionClose()
{
    emit sessionClosed("Disconnect. Session closed \n");
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
    // если сокет дождался соединения с сервером
    if (socket->waitForConnected()){
        //qDebug() <<"We have connect to server!!! send message";
        //protocolOut MessageOut(baMessage);
        //socket->write(MessageOut.getMessageToClient());
//        qDebug() << "jdQuery" << jdQuery;
//        out->setPackage(jdQuery);
//         qDebug() << "out->getPackage()" << out->getPackage();

        // запись выходного пакета в сокет
        socket->write(out->getPackage());

        // ожидание ответа

        // если ответ не получен в течение 5 секунд
        if (!socket->waitForReadyRead(5000)) {
            // формируем ответ
//            QVariantMap mapCommand;
//            mapCommand["codeCommand"] = setCodeCommand::TimeOut;
//            mapCommand["joDataInput"] = "server timeout";
//            jdRespond = QJsonDocument::fromVariant(mapCommand);

//            codeCommand = setCodeCommand::TimeOut;
//            QJsonObject joTemp;
//            joTemp.insert("codeCommand",setCodeCommand::TimeOut);
//            joTemp.insert("joDataInput","server timeout");
            emit sessionClosed("Problem: server time out \n");
            //qDebug() << "time out";
        }
//        else{
//            qDebug() << "not time out";
//        }
    }
    else{
        //qDebug() << "client say - no connect";

//        // формуруем ответ в виде JSON- документа
//        QVariantMap mapCommand;
//        mapCommand["codeCommand"] = setCodeCommand::NoConnect;
//        mapCommand["joDataInput"] = "no connection to server";
//        jdRespond = QJsonDocument::fromVariant(mapCommand);

//        joRespond.insert("codeCommand",NoConnect);
//        joRespond.insert("joDataInput","no connection to server");
        emit serverResponded("Problem: no connection to server \n");
    }
}

