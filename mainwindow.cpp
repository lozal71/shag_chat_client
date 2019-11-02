#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDataStream>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client = new chatClient();
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
    // клик по кнопке Авторизации - сбор данных для авторизации
    connect(ui->pbAuth, &QPushButton::clicked,
            this,  &MainWindow::collectDataAuth);
    // данные собраны - послать запрос
    connect(this,&MainWindow::dataAuthCollected,
            client, &chatClient::prepareQueryAuth);
    connect(client,&chatClient::serverResponded,
            this,&MainWindow::logServerResponds);
    connect(client, &chatClient::sessionClosed,
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
                qDebug() << "joRespond.value(\"list_rooms_id\")" <<joRespond.value("list_rooms_id");
                QJsonArray arr = joRespond.value("list_rooms_id").toArray();
                qDebug() << "arr" << arr;
                QList<QVariant> arr1 = arr.toVariantList();
                qDebug() << "arr" << arr;
                for (auto elem: arr1) {
                    qDebug() << "elem" << elem;
                    ui->teLog->insertPlainText(QString::number(elem.toInt()) + "\n");
                }
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

void MainWindow::collectDataAuth()
{
    client->setLogin(ui->cbxLogins->currentText());
    client->setPass(ui->cbxPasswords->currentText());
    emit dataAuthCollected();
}



