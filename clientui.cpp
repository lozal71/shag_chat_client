#include "clientui.h"
#include "ui_clientui.h"

clientUI::clientUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::clientUI)
{
    ui->setupUi(this);
}

clientUI::~clientUI()
{
    delete ui;
}
