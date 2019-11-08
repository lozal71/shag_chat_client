#include "protocol_out.h"


protocolOut::protocolOut()
{

}

QByteArray protocolOut::getPackage()
{
    return baPackage;
}

void protocolOut::setPackage(QJsonDocument jdParam)
{
    //qDebug() << "jdParam" << jdParam;
    QByteArray baTemp = jdParam.toJson(QJsonDocument::Compact);
    //quint32 packageSize = str.length();
   // qDebug() << "baTemp" << baTemp;
    quint32 packageSize = quint32(baTemp.size());
    //qDebug() << "packageSize" << packageSize;
    QDataStream stream(&baPackage, QIODevice::ReadWrite);
    stream << packageSize;
    stream << baTemp;
}
