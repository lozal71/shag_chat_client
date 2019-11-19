#include "dialogauth.h"
#include "ui_dialogauth.h"

DialogAuth::DialogAuth(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAuth)
{
    ui->setupUi(this);
    fullCbxLogins();
    fullCbxPasswords();
    authAgain = false;
    flagExit = false;
    //qDebug() << "constructur flagExit" << flagExit;
}

DialogAuth::~DialogAuth()
{
    //qDebug() << "deconstructur flagexit" << flagExit;
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
    return ui->cbxLogin->currentText();
}

QString DialogAuth::getPass()
{
    return ui->cbxPass->currentText();
}

void DialogAuth::showWarningAuth(QString sParam)
{
    int r;
    // сообщаем пользователю о проблеме авторизации
    // и спрашиваем хочет ли он попробовать еще раз
    QMessageBox quest(QMessageBox::Question,"Question",sParam);
    quest.setStandardButtons(QMessageBox::Yes| QMessageBox::No);
    r=quest.exec();
    if(r == QMessageBox::Yes) {
        authAgain = true;
        flagExit = false;
        //qDebug() << "yes";
    }
    else {
        authAgain = false;
        flagExit = true;
        //qDebug() << "no flagExit" << flagExit;
    }
}

void DialogAuth::on_buttonBox_rejected()
{
    //qDebug() << "on_buttonBox_rejected";
    flagExit = true;
    //QApplication::exit();
}
