#include "roombutton.h"

RoomButton::RoomButton()
{

}

RoomButton::RoomButton(int ID, QString sName, QVariantMap mapUserMess)
{
    room.id = ID;
    room.name = sName;
    room.mapMess = mapUserMess;
    setText(sName);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
}

int RoomButton::getRoomID()
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
