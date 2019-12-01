#ifndef NOTIFYBUTTON_H
#define NOTIFYBUTTON_H

//#include <QtCore/qglobal.h>
#include <QDebug>
#include <QVariantMap>
#include <QPushButton>


class NotifyButton: public QPushButton
{
    Q_OBJECT
public:
    NotifyButton();
    NotifyButton(int cbxIndex, QVariantMap mapInvite, int inviteID);
    void setParam(int cbxIndex, QVariantMap mapInvite, int inviteID);
    void setNull();
    int getIndex();
    QVariantMap getMap();
    int getInviteID();

private:
    struct inviteData {
        int cbxIndex;
        QVariantMap mapInvite;
        int inviteID;
    } btnInvite;
};

#endif // NOTIFYBUTTON_H
