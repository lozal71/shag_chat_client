#include "protocol_out.h"

protocolOut::protocolOut(){
	
	

}

protocolOut::protocolOut(QByteArray &str)
{
    quint32 packageSize = str.length();
    QDataStream stream(&block, QIODevice::ReadWrite);
    stream << packageSize;
    stream << str;




//   QJsonObject jobject;
//   jobject["login"] = ui->leLogin->text();
//   jobject["commandCode"] = commandCode::auth; //для авторизации логин пароль
//   jobject["password"]= ui->lePassword->text();
//   QJsonDocument doc(jobject);
//   client->sendData(doc.toJson());
}

QByteArray protocolOut::getMessageToClient()
{
    return block;
}
