#include "dialogauth.h"
#include "ui_dialogauth.h"

DialogAuth::DialogAuth(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAuth)
{
    ui->setupUi(this);

}

DialogAuth::~DialogAuth()
{
    delete ui;
}
