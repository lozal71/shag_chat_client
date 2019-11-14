#ifndef ROOMBUTTON_H
#define ROOMBUTTON_H
#include <QDebug>
#include <QVariantMap>
#include <QPushButton>

class RoomButton: public QPushButton
{
    Q_OBJECT
public:
    RoomButton(int ID, int role, QString sName, QVariantMap mapUserMess);
    int getRoomID();
    int getRole();
    QString getRoomName();
    QVariantMap getMapUserMess();
    void catchRoomClick();
    void setMapMess(QVariantMap mapParam);
    void debugMapMess(QVariantMap mapMess);
private:
    struct roomData {
        int id;
        int role;
        QString name;
        QVariantMap mapMess;
    } room;

signals:
    void roomClicked(QVariantMap mapRooms);
};

#endif // ROOMBUTTON_H
