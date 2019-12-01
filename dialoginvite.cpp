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

void DialogInvite::fullCbxUserName(QStringList listUsers)
{
    ui->cbxUserName->clear();
    ui->cbxUserName->addItems(listUsers);
}

void DialogInvite::fullCbxTextInvite(QStringList listText)
{
    ui->cbxTextInvite->clear();
    ui->cbxTextInvite->addItems(listText);
}
