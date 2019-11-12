#ifndef ROOMBUTTON_H
#define ROOMBUTTON_H
#include <QDebug>
#include <QVariantMap>
#include <QPushButton>

class RoomButton: public QPushButton
{
    Q_OBJECT
public:
    RoomButton();
    RoomButton(int sID, QString sName, QVariantMap mapUserMess);
    int getRoomID();
    QString getRoomName();
    QVariantMap getMapUserMess();
    void catchRoomClick();
private:
    struct roomData {
        int id;
        QString name;
        QVariantMap mapMess;
    } room;
signals:
    void roomClicked(QVariantMap mapRooms);
};

#endif // ROOMBUTTON_H
