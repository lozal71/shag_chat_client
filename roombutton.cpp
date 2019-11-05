#include "roombutton.h"

RoomButton::RoomButton()
{

}

RoomButton::RoomButton(QString sID, QString sName, QVariantMap mapUserMess)
{
    room.id = sID;
    room.name = sName;
    room.mapMess = mapUserMess;
}

QString RoomButton::getRoomID()
{
    return room.id;
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
