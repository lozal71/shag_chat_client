#include "protocol_in.h"

protocolIn::protocolIn(){
    flag_error = false;
}

 QJsonDocument protocolIn::receiveJSONdoc(QTcpSocket *socket)
{
    // qDebug() << "receiveJSONdoc";
    QJsonDocument jdTemp;
    flag_error = false;
    // если в сокете меньше, чем 4 байта
    if (socket->bytesAvailable() < 4){
        flag_error = true;
     }
    else
    {
        quint32 packageSize; // размер пришедшего сообщения
        quint32 baBufferLength = 0; // размер буфера
        qint64 bytesInSocket = 0;  // число байтов в сокете
        QByteArray baBuffer;
        baBuffer.clear();
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
        // qDebug() << "jdTemp " <<jdTemp;
    }
    return jdTemp;
 }

 bool protocolIn::isError()
 {
     return flag_error;
 }

