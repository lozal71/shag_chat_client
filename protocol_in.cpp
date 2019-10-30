#include "protocol_in.h"



//protocolIn::protocolIn(){
//}

protocolIn::protocolIn(QTcpSocket *socket)
{
    if (socket->bytesAvailable()< 4){
        codeCommand = ErrorMessage;
        QJsonObject joMessage;
        joMessage.insert("codeCommand",setCodeCommand::ErrorMessage);
        joMessage.insert("joDataInput", "error message");
    }
    else{
        QJsonObject joMessage = getJsonObjectIN(socket);
        codeCommand = setCodeCommand(joMessage.value("codeCommand").toInt());
        joDataInput = joMessage.value("joDataInput").toObject();
    }
}

setCodeCommand protocolIn::getCode()
{
    return codeCommand;
}

QJsonObject protocolIn::getData()
{
    return joDataInput;
}

QJsonObject protocolIn::getJsonObjectIN(QTcpSocket *socket)
{
    quint32 packageSize; // размер приходящего сообщения
    quint32 baBufferLength = 0;
    qint64 bytesInSocket = 0;
    QByteArray baBuffer;
    baBuffer = socket->read(4); // считываем 4 байта
    QDataStream stream(baBuffer);
    stream >> packageSize; // получаем размер приходящего сообщения
    //qDebug() << "packageSize " << packageSize;
    packageSize+=4;
    baBuffer.clear();
    baBufferLength = quint32(baBuffer.length());
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
    baBuffer.remove(0, 4);
    //qDebug() << "buffer в конце " << buffer;
    QString sBuffer(baBuffer);
    QJsonDocument jsonDoc = QJsonDocument::fromJson(sBuffer.toUtf8());
    QJsonObject joInput = jsonDoc.object();
    return joInput;
}
