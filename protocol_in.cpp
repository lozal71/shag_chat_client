#include "protocol_in.h"



protocolIn::protocolIn(){
    flag_error = false;
}

//protocolIn::protocolIn(QTcpSocket *socket)
//{
//    if (socket->bytesAvailable()< 4){
//        QVariantMap mapCommand;
//        mapCommand["codeCommand"] = setCodeCommand::ErrorMessage;
//        mapCommand["joDataInput"] = "error message";
//        jdRespond = QJsonDocument::fromVariant(mapCommand);

////        codeCommand = ErrorMessage;
////        QJsonDocument joMessage;
////        joMessage.insert("codeCommand",setCodeCommand::ErrorMessage);
////        joMessage.insert("joDataInput", "error message");
//    }
//    else{

//        QJsonObject joMessage = getJsonObjectIN(socket);
//        codeCommand = setCodeCommand(joMessage.value("codeCommand").toInt());
//        joDataInput = joMessage.value("joDataInput").toObject();
//    }
//}

//setCodeCommand protocolIn::getCode()
//{
//    return codeCommand;
//}

//QJsonObject protocolIn::getData()
//{
//    return joDataInput;
//}

 QJsonDocument protocolIn::receiveJSONdoc(QTcpSocket *socket)
{
    //QVariantMap mapTemp;
    //mapTemp.clear();
    QJsonDocument jdTemp;
    flag_error = false;
    // если в сокете меньше, чем 4 байта
    if (socket->bytesAvailable() < 4){
        flag_error = true;
        // формируем JSON-документ с сообщением об ошибке

//        mapTemp["codeCommand"] = setCodeCommand::ErrorMessage;
//        mapTemp["joDataInput"] = "error message";
        //jdRespond = QJsonDocument::fromVariant(mapCommand);

//        codeCommand = ErrorMessage;
//        QJsonDocument joMessage;
//        joMessage.insert("codeCommand",setCodeCommand::ErrorMessage);
//        joMessage.insert("joDataInput", "error message");
    }
    else
    {

        //QJsonObject joMessage = getJSONdoc(socket);


        quint32 packageSize; // размер пришедшего сообщения
        quint32 baBufferLength = 0; // размер буфера
        qint64 bytesInSocket = 0;  // число байтов в сокете
        QByteArray baBuffer;
        baBuffer = socket->read(4); // считываем 4 байта
        QDataStream stream(baBuffer); // назначаем baBuffer получателем данных стрима
        stream >> packageSize; // считываем размер пришедшего сообщения
        //qDebug() << "packageSize " << packageSize;
        packageSize+=4; // прибавляем 4 байта, чтобы отсечь длину сообщения от данных
        baBuffer.clear();
//        baBufferLength = quint32(baBuffer.length());
        // узнаем сколько сейчас доступно байт в сокете
        bytesInSocket = socket->bytesAvailable();
        // пока не получены все байты сообщения
        while (true) {
            // если число доступных байт в сокете меньше или равно, чем ожидаемый остаток
            if (bytesInSocket <= packageSize - baBufferLength){
                baBuffer.append(socket->readAll()); // читаем из буфера все, что есть
                //qDebug() << "buffer в цикле  " << buffer;
            }
            else  {
                    // читаем только остаток
                    baBuffer.append(socket->read(packageSize - baBufferLength));
                }
            baBufferLength += quint32(baBuffer.length());
            bytesInSocket = socket->bytesAvailable();
            if (baBufferLength == packageSize) break;
        }
        baBuffer.remove(0, 4); // отсекаем длину сообщения
        //qDebug() << "buffer в конце " << buffer;
        QString sBuffer(baBuffer);
        jdTemp = QJsonDocument::fromJson(sBuffer.toUtf8());

         qDebug() << "jdTemp " <<jdTemp;

//        mapTemp["codeCommand"] = setCodeCommand::Auth;
//        mapTemp["joDataInput"] = jdBuffer;
        //jdRespond = QJsonDocument::fromVariant(mapCommand);
//        return jsonDoc;



//        codeCommand = setCodeCommand(joMessage.value("codeCommand").toInt());
//        joDataInput = joMessage.value("joDataInput").toObject();
    }
    return jdTemp;
 }

 bool protocolIn::isError()
 {
     return flag_error;
 }

//QVariantMap protocolIn::getMAP()
//{
//    return mapRespond;
//}

//QJsonDocument protocolIn::getJSONdoc(QTcpSocket *socket)
//{
//    quint32 packageSize; // размер приходящего сообщения
//    quint32 baBufferLength = 0;
//    qint64 bytesInSocket = 0;
//    QByteArray baBuffer;
//    baBuffer = socket->read(4); // считываем 4 байта
//    QDataStream stream(baBuffer);
//    stream >> packageSize; // получаем размер приходящего сообщения
//    //qDebug() << "packageSize " << packageSize;
//    packageSize+=4;
//    baBuffer.clear();
//    baBufferLength = quint32(baBuffer.length());
//    bytesInSocket = socket->bytesAvailable();
//    // пока не получены все байты сообщения
//    while (true) {
//        // если число доступных байт в сокете меньше или равно, чем ожидаемый остаток
//        if (bytesInSocket <= packageSize - baBufferLength){
//            baBuffer.append(socket->readAll()); // читаем из буфера все, что есть
//            //qDebug() << "buffer в цикле  " << buffer;
//        }
//        else  {
//                // читаем только остаток
//                baBuffer.append(socket->read(packageSize - baBufferLength));
//            }
//        baBufferLength += quint32(baBuffer.length());
//        bytesInSocket = socket->bytesAvailable();
//        if (baBufferLength == packageSize) break;
//    }
//    baBuffer.remove(0, 4);
//    //qDebug() << "buffer в конце " << buffer;
//    QString sBuffer(baBuffer);
//    QJsonDocument jsonDoc = QJsonDocument::fromJson(sBuffer.toUtf8());
//    return jsonDoc;
//}
