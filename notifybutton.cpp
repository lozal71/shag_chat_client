#include "notifybutton.h"

NotifyButton::NotifyButton()
{
    setEnabled(false);
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    setText("посмотреть");
    btnInvite.cbxIndex = -1;
    btnInvite.mapInvite.clear();
    btnInvite.inviteID = -1;
}

NotifyButton::NotifyButton(int cbxIndex, QVariantMap mapInvite, int inviteID)
{
    btnInvite.cbxIndex = cbxIndex;
    btnInvite.mapInvite = mapInvite;
    btnInvite.inviteID = inviteID;

}

void NotifyButton::setNull()
{
    setText("посмотреть");
    btnInvite.cbxIndex = -1;
    btnInvite.mapInvite.clear();
    btnInvite.inviteID = -1;
    setEnabled(false);
}

void NotifyButton::setParam(int cbxIndex, QVariantMap mapInvite, int inviteID)
{
    btnInvite.cbxIndex = cbxIndex;
    btnInvite.mapInvite = mapInvite;
    btnInvite.inviteID = inviteID;
}

int NotifyButton::getIndex()
{
    return btnInvite.cbxIndex;
}

QVariantMap NotifyButton::getMap()
{
    return btnInvite.mapInvite;
}

int NotifyButton::getInviteID()
{
    return btnInvite.inviteID;
}



