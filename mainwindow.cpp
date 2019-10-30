#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDataStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    clientSocket = new chatClient();
    setConnect();
    fullCbxLogins();
    fullCbxPasswords();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setConnect()
{

    connect(ui->pbAuth, &QPushButton::clicked,
            this,  &MainWindow::slotSendQueryAuth);
    connect(this,  SIGNAL(SendQueryAuth(QJsonObject)),
            clientSocket, SLOT(slotSendQuery(QJsonObject)));
    connect(clientSocket,&chatClient::serverResponded,
            this,&MainWindow::logServerResponds);
    connect(clientSocket, &chatClient::sessionClosed,
            this, &MainWindow::logServerResponds);
}

void MainWindow::logServerResponds(setCodeCommand code, QJsonObject joRespond)
{
    ui->teLog->insertPlainText("Respond from server:\n");
    switch (code) {
        case setCodeCommand::Auth:
        {
            qDebug() << "joRespond.value(\"id\").toInt()" << joRespond.value("id").toInt();
            if (joRespond.value("id").toInt() != 0){
                ui->teLog->insertPlainText(QString::number(joRespond.value("id").toInt())+"\n");
                ui->teLog->insertPlainText(joRespond.value("name").toString() + "\n");
            }
            else{
                ui->teLog->insertPlainText("not correct login or password\n");
            }

        }
        break;
        case setCodeCommand::ErrorMessage:
        {
            ui->teLog->insertPlainText(joRespond.value("joDataInput").toString()+"\n");
        }
        break;
        case setCodeCommand::NoConnect:{
             ui->teLog->insertPlainText(joRespond.value("joDataInput").toString()+"\n");
        }
            break;
        case setCodeCommand::SessionClosed:{
             ui->teLog->insertPlainText(joRespond.value("joDataInput").toString()+"\n");
        }
            break;
         case setCodeCommand::TimeOut:{
             ui->teLog->insertPlainText(joRespond.value("joDataInput").toString()+"\n");
        }
            break;
    }

//    for (const QString& eachKey : joRespond.keys())
//    {
//        ui->teLog->insertPlainText(joRespond.value(eachKey).toString()+"\n");

//    }
}


void MainWindow::slotSendQueryAuth()
{
    QJsonObject joTemp;
    joTemp.insert("login",ui->cbxLogins->currentText());
    joTemp.insert("pass",ui->cbxPasswords->currentText());
    emit SendQueryAuth(joTemp);
}


void MainWindow::fullCbxLogins()
{
    ui->cbxLogins->addItem("login1");
    ui->cbxLogins->addItem("login2");
    ui->cbxLogins->addItem("login3");
}

void MainWindow::fullCbxPasswords()
{
    ui->cbxPasswords->addItem("pass1");
    ui->cbxPasswords->addItem("pass2");
    ui->cbxPasswords->addItem("pass3");
}



