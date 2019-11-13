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
