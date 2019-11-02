#include "protocol.h"

protocol::protocol()
{

}

QJsonDocument protocol::getJsonDoc(QString *str, ...)
{
    QVariantMap mapTemp;
    QString *key;
    QString *value;
    va_list arg_ptr;           // Объявление указателя на список аргументов
    va_start(arg_ptr, str);
    while (str != nullptr)
        {
            key = va_arg(arg_ptr, QString*);
            value = va_arg(arg_ptr, QString*);
            mapTemp[*key] = *value;
        }
        va_end(arg_ptr);
    //QVariantMap mapTemp;
//    mapTemp[ui->lbLogin->text()] = ui->cbxLogins->currentText();
//    mapTemp[ui->lbPassword->text()] = ui->cbxPasswords->currentText();
    QJsonDocument jdTemp = QJsonDocument::fromVariant(mapTemp);
    return jdTemp;
}
