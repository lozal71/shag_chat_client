#include "dialoginvite.h"
#include "ui_dialoginvite.h"


DialogInvite::DialogInvite(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogInvite)
{
    ui->setupUi(this);
}

DialogInvite::~DialogInvite()
{
    delete ui;
}

QString DialogInvite::getUserName()
{
    return ui->cbxUserName->currentText();
}

QString DialogInvite::getTextInvite()
{
    return ui->cbxTextInvite->currentText();
}

void DialogInvite::fullCbxUserName()
{
    ui->cbxUserName->addItem("user1");
    ui->cbxUserName->addItem("user2");
    ui->cbxUserName->addItem("user3");
}

void DialogInvite::fullCbxTextInvite()
{
    ui->cbxTextInvite->addItem("text1");
    ui->cbxTextInvite->addItem("tex2");
    ui->cbxTextInvite->addItem("text3");
}
