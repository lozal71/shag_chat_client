#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client = new chatClient();
    connectClientUI();
    fullCbxLogins();
    fullCbxPasswords();
}

MainWindow::~MainWindow()
{
    delete client;
    delete ui;
}

void MainWindow::connectClientUI()
{
    // клик по кнопке Авторизации - сбор данных для авторизации
    connect(ui->pbAuth, &QPushButton::clicked,
            this,  &MainWindow::collectDataAuth);
    // данные для Авторизации собраны - клиент готовит запрос на Авторизацию
    connect(this,&MainWindow::dataAuthCollected,
            client, &chatClient::prepareQueryAuth);
    // сервер ответил - выводим ответ в окно логирования
    connect(client,&chatClient::serverResponded,
            this,&MainWindow::logServerResponds);
    // сессия закрылась - выводим сообщение в окно логирования
    connect(client, &chatClient::sessionClosed,
            this, &MainWindow::logServerResponds);
}

void MainWindow::logServerResponds(QString sParam)
{

    ui->teLog->insertPlainText("Respond from server:\n");
    ui->teLog->insertPlainText(sParam);
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



