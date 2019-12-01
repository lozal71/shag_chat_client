#include "roombutton.h"

RoomButton::RoomButton(int ID,
                       QString role, QString sName,
                       QVariantMap mapMess, QVariantMap mapUsers)
{
    room.id = ID;
    room.role = role;
    room.name = sName;
    room.mapMess = mapMess;
    room.mapUsers = mapUsers;
    setText(sName + " " + role);
    room.listCastMess.clear();
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
}


int RoomButton::getRoomID()
{
    return room.id;
}

QString RoomButton::getRole()
{
    return room.role;
}

QString RoomButton::getRoomName()
{
    return room.name;
}

QVariantMap RoomButton::getMapMess()
{
    return room.mapMess;
}

QVariantMap RoomButton::getMapUsers()
{
    return room.mapUsers;
}

void RoomButton::catchRoomClick()
{
    emit roomClicked(room.mapMess);
}

void RoomButton::setMapMess(QVariantMap mapParam)
{
    room.mapMess = mapParam;
}

void RoomButton::setMapUsers(QVariantMap mapParam)
{
    room.mapUsers = mapParam;
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

QList<QVariantMap> RoomButton::getListCastMess()
{
    return room.listCastMess;
}

void RoomButton::setListCastMess(QList<QVariantMap> listCast)
{
    room.listCastMess = listCast;
}

void RoomButton::updateCastMess(QVariantMap mapMess)
{
    room.listCastMess.append(mapMess);
}

void RoomButton::updateMapUsers(QString userID, QString senderName, setUpdateUsers param)
{
    if (param == setUpdateUsers::addUser) {
        room.mapUsers.insert(userID,senderName);
    }
    else if (param == setUpdateUsers::delIsYou){
        room.mapUsers.clear();
        setStyleSheet("font: 14px; color: black; background-color: yellow");
    }
    else {
        room.mapUsers.remove(userID);
    }
}


