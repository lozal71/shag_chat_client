#include "net.h"

net::net()
{

}

net::net(QTcpSocket *socket)
{
    this->socket = socket;
}

void net::writeSocket(QVariantMap mapSocket)
{
    qDebug() << "writeSocket";
    jsonDoc = QJsonDocument::fromVariant(mapSocket);
    transferJSONtoBA();
    this->socket->write(baPackage);
}

QVariantMap net::readSocket()
{
    QVariantMap mapCommand;
    transferBAtoJSON();
    QJsonObject joTemp = jsonDoc.object();
    return mapCommand =joTemp.toVariantMap();
}

void net::transferBAtoJSON()
{
    flag_error = false;
    // если в сокете меньше, чем 4 байта
    if (socket->bytesAvailable() < 4){
        flag_error = true;
     }
    else
    {
        quint32 packageSize = 0; // размер пришедшего сообщения
        quint32 baBufferLength = 0; // размер буфера
        qint64 bytesInSocket = 0;  // число байтов в сокете
        QByteArray baBuffer;
        baBuffer.clear();
        baBuffer = socket->read(4); // считываем 4 байта
        //qDebug() << "baBuffer" << baBuffer;
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
        //        qDebug() << "buffer в цикле  " << baBuffer;
            }
            else  {
                    // читаем только остаток
                    baBuffer.append(socket->read(packageSize - baBufferLength));
                }
            baBufferLength += quint32(baBuffer.length());
            //qDebug() << "baBufferLength" << baBufferLength;
            bytesInSocket = socket->bytesAvailable();
            if (baBufferLength == packageSize) break;
        }
        baBuffer.remove(0, 4); // отсекаем длину сообщения
        //qDebug() << "buffer в конце " << baBuffer;
        QString sBuffer(baBuffer);
        jsonDoc = QJsonDocument::fromJson(sBuffer.toUtf8());
//         qDebug() << "jdTemp " <<jdTemp;
    }
    //return jdTemp;
}

bool net::isError()
{
    return flag_error;
}

void net::transferJSONtoBA()
{
    baPackage.clear();
    qDebug() << "jsonDoc" << jsonDoc;
    QByteArray baTemp = jsonDoc.toJson(QJsonDocument::Compact);
    quint32 packageSize = quint32(baTemp.size());
    qDebug() << "packageSize" << packageSize;
    QDataStream stream(&baPackage, QIODevice::ReadWrite);
    stream << packageSize;
    qDebug() << "setPackage baPackage" <<baPackage ;
    stream << baTemp;
    qDebug() << "setPackage baPackage" <<baPackage ;
}
