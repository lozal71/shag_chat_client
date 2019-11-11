#include "dialogauth.h"
#include "ui_dialogauth.h"

DialogAuth::DialogAuth(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAuth)
{
    ui->setupUi(this);
    fullCbxLogins();
    fullCbxPasswords();

}

DialogAuth::~DialogAuth()
{
    delete ui;
}

void DialogAuth::fullCbxLogins()
{

    ui->cbxLogin->addItem("login1");
    ui->cbxLogin->addItem("login2");
    ui->cbxLogin->addItem("login3");
}

void DialogAuth::fullCbxPasswords()
{
    ui->cbxPass->addItem("pass1");
    ui->cbxPass->addItem("pass2");
    ui->cbxPass->addItem("pass3");
}

QString DialogAuth::getLogin()
{
    return login;
}

QString DialogAuth::getPass()
{
    return pass;
}

void DialogAuth::on_buttonBox_accepted()
{
    login = ui->cbxLogin->currentText();
    pass = ui->cbxPass->currentText();
}
