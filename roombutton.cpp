#include "roombutton.h"

RoomButton::RoomButton(int ID,
                       QString role, QString sName,
                       QVariantMap mapMess, QVariantMap mapUsers, QWidget *parent):
    QPushButton(parent)
{
    room.id = ID;
    room.role = role;
    room.name = sName;
    room.mapMess = mapMess;
    room.mapUsers = mapUsers;
    setText(sName + " " + role);
    room.listCastMess.clear();
    this->setMinimumHeight(100);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
//    this->setContextMenuPolicy(Qt::ActionsContextMenu);
//    QAction *actDelRoom = new QAction(QObject::tr("del room"), this);
//    QAction *actAddRoom = new QAction(QObject::tr("add room"), this);
//    this->addAction(actDelRoom);
//    this->addAction(actAddRoom);
//    connect(actDelRoom, SIGNAL(triggered()),
//                            this, SLOT(slotDelRoom()));
//    connect(actAddRoom, SIGNAL(triggered()),
//                            this, SLOT(slotAddRoom()));
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


