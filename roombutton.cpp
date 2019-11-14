#include "roombutton.h"

RoomButton::RoomButton(int ID, int role, QString sName, QVariantMap mapUserMess)
{
    room.id = ID;
    room.role = role;
    room.name = sName;
    room.mapMess = mapUserMess;
    if (role == 1)  {
        setText(sName + " admin");
    }
    else {
        setText(sName + " user");
    }
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
}

int RoomButton::getRoomID()
{
    return room.id;
}

int RoomButton::getRole()
{
    return room.role;
}

QString RoomButton::getRoomName()
{
    return room.name;
}

QVariantMap RoomButton::getMapUserMess()
{
    return room.mapMess;
}

void RoomButton::catchRoomClick()
{
    emit roomClicked(room.mapMess);
}

void RoomButton::setMapMess(QVariantMap mapParam)
{
    room.mapMess = mapParam;
}

void RoomButton::debugMapMess(QVariantMap mapTimeNameMess)
{
    for (const QString& time: mapTimeNameMess.keys()){
        QVariantMap mapNameMess = mapTimeNameMess[time].toMap();
        qDebug() << time << ": ";
        for (const QString& name: mapNameMess.keys()) {
            qDebug() << name << ": "<< mapNameMess[name].toString();
        }

    }
}
