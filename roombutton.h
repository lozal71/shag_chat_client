#ifndef ROOMBUTTON_H
#define ROOMBUTTON_H
#include <QDebug>
#include <QVariantMap>
#include <QPushButton>
#include <QList>
#include <QScrollArea>
//#include <QAction>

enum setUpdateUsers {addUser, removeUser, delIsYou};

class RoomButton: public QPushButton
{
    Q_OBJECT
public:
    //explicit RoomButton(QWidget *parent = nullptr);
    RoomButton(int ID, QString role, QString sName, QVariantMap mapMess, QVariantMap mapUsers, QWidget *parent = nullptr);
    int getRoomID();
    QString getRole();
    QString getRoomName();
    QVariantMap getMapMess();
    QVariantMap getMapUsers();
    void catchRoomClick();
    void setMapMess(QVariantMap mapParam);
    void setMapUsers(QVariantMap mapParam);
    void debugMapMess(QVariantMap mapMess);
    QList<QVariantMap> getListCastMess();
    void setListCastMess(QList<QVariantMap> listCast);
    void updateCastMess(QVariantMap mapMess);
    void updateMapUsers(QString userID, QString senderName, setUpdateUsers param);
private:
    struct roomData {
        int id;
        QString role;
        QString name;
        QVariantMap mapMess;
        QVariantMap mapUsers;
        QList<QVariantMap> listCastMess;
    } room;

signals:
    void roomClicked(QVariantMap mapRooms);
};

#endif // ROOMBUTTON_H
