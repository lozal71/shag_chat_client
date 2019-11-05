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
    connect(client,&chatClient::serverRespondedLog,
            this,&MainWindow::logServerResponds);
    connect(client,&chatClient::serverRespondedLog,
            this,&MainWindow::showName);
    // сессия закрылась - выводим сообщение в окно логирования
    connect(client, &chatClient::sessionClosed,
            this, &MainWindow::logServerResponds);
    connect(client, &chatClient::serverRespondedMap,
            this, &MainWindow::showRooms);
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

void MainWindow::showRooms(QVariantMap mapRooms)
{
    //qDebug() << "mapRooms" << mapRooms;
    for (const QString& roomID: mapRooms.keys()){
        QVariantMap mapRoomName = mapRooms[roomID].toMap();
        for (const QString& roomName: mapRoomName.keys()) {
            RoomButton *btnRoom = new RoomButton(roomID,roomName,mapRoomName[roomName].toMap());
            connect (btnRoom, &RoomButton::clicked, this, &MainWindow::showMessage);
            mapRoomButton[btnRoom] = btnRoom->getRoomID();
            btnRoom->setText(roomName);
            ui->vltListRooms->addWidget(btnRoom);
        }
    }
}

void MainWindow::showName()
{
    ui->pbAuth->setEnabled(false);
    QLabel *lblName = new QLabel(client->getName());
    ui->hltNameUser->addWidget(lblName);
}

void MainWindow::showMessage()
{
    ui->teChat->clear();
    QString sTemp="";
    QVariantMap mapTimeMess = static_cast<RoomButton*>(sender())->getMapUserMess();
    qDebug() << "clicked from room button, room id is " << mapTimeMess;
    for (const QString& timeMess: mapTimeMess.keys()){
        sTemp += timeMess + ": ";
        QVariantMap mapSenderMessage = mapTimeMess[timeMess].toMap();
        for (const QString& sender: mapSenderMessage.keys()){
            sTemp += "from " + sender + "\n" + mapSenderMessage[sender].toString() +"\n";
            qDebug() <<"sTemp" << sTemp;
            ui->teChat->insertPlainText(sTemp);
        }
        sTemp.clear();
    }

}



