#ifndef ROOMBUTTON_H
#define ROOMBUTTON_H
#include <QDebug>
#include <QVariantMap>
#include <QPushButton>
#include <QList>

class RoomButton: public QPushButton
{
    Q_OBJECT
public:
    RoomButton(int ID, QString role, QString sName, QVariantMap mapUserMess);
    int getRoomID();
    QString getRole();
    QString getRoomName();
    QVariantMap getMapUserMess();
    void catchRoomClick();
    void setMapMess(QVariantMap mapParam);
    void debugMapMess(QVariantMap mapMess);
    QList<QVariantMap> getListCastMess();
    void setListCastMess(QList<QVariantMap> listCast);
private:
    struct roomData {
        int id;
        QString role;
        QString name;
        QVariantMap mapMess;
        QList<QVariantMap> listCastMess;
    } room;

signals:
    void roomClicked(QVariantMap mapRooms);
};

#endif // ROOMBUTTON_H
