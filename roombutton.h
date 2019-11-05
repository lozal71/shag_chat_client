#ifndef ROOMBUTTON_H
#define ROOMBUTTON_H
#include <QDebug>
#include "ui_mainwindow.h"
#include <QVariantMap>

class RoomButton: public QPushButton
{
    Q_OBJECT
public:
    RoomButton();
    RoomButton(QString sID, QString sName, QVariantMap mapUserMess);
    QString getRoomID();
    QString getRoomName();
    QVariantMap getMapUserMess();
    void catchRoomClick();
private:
    struct roomData {
        QString id;
        QString name;
        QVariantMap mapMess;
    } room;
signals:
    void roomClicked(QVariantMap mapRooms);
};

#endif // ROOMBUTTON_H
