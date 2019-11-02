#include "protocol_out.h"


protocolOut::protocolOut()
{

}

//protocolOut::protocolOut(QJsonDocument &jdTemp)
//{
////    QByteArray baMessage = jdTemp.toJson(QJsonDocument::Compact);
////    //quint32 packageSize = str.length();
////    quint32 packageSize = quint32(baMessage.length());
////    QDataStream stream(&package, QIODevice::ReadWrite);
////    stream << packageSize;
////    stream << baMessage;

////   QJsonObject jobject;
////   jobject["login"] = ui->leLogin->text();
////   jobject["commandCode"] = commandCode::auth; //для авторизации логин пароль
////   jobject["password"]= ui->lePassword->text();
////   QJsonDocument doc(jobject);
////   client->sendData(doc.toJson());
//}

QByteArray protocolOut::getPackage()
{
    return baPackage;
}

void protocolOut::setPackage(QJsonDocument jdParam)
{
    qDebug() << "jdParam" << jdParam;
    QByteArray baTemp = jdParam.toJson(QJsonDocument::Compact);
    //quint32 packageSize = str.length();
    qDebug() << "baTemp" << baTemp;
    quint32 packageSize = quint32(baTemp.size());
    qDebug() << "packageSize" << packageSize;
    QDataStream stream(&baPackage, QIODevice::ReadWrite);
    stream << packageSize;
    stream << baTemp;
}
