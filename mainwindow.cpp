#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client = new chatClient();
    uiLog = new DialogAuth();
    connectClientUI();
    fullCbxLogins();
    fullCbxPasswords();
    ui->leWriteMes->setStyleSheet("color: gray");
    ui->leWriteMes->setText("Please, write here...");

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
    // сессия закрылась - выводим сообщение в окно логирования
    connect(client, &chatClient::sessionClosed,
            this, &MainWindow::logServerResponds);
    // клиент сообщил, что сервер прислал ответ на авторизацию
        // - показываем комнаты и имя пользователя
    connect(client, &chatClient::serverRespondedAuth,
            this, &MainWindow::showRoomsUserName);
    // клик по кнопке-Send - собираем данные для отправки сообщения
    connect(ui->pbSend, &QPushButton::clicked,
            this, &MainWindow::collectDataSend);
    // данные для отправки сообщения собраны - клиент готовит запрос - "отправка сообщения"
    connect(this, &MainWindow::dataSendCollected,
            client, &chatClient::prepareQuerySendMessage);
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

void MainWindow::collectDataSend()
{
    //qDebug() << "ui->leWritrMes->text()" <<ui->leWriteMes->text() ;
    if (!ui->leWriteMes->text().isEmpty()){
        client->setText(ui->leWriteMes->text());
        ui->teChat->setAlignment(Qt::AlignRight);
        ui->teChat->insertPlainText( ui->leWriteMes->text() +"\n");
        ui->leWriteMes->clear();
       // ui->pbSend->setEnabled(false);
        emit dataSendCollected(roomActivID);
    }
}

void MainWindow::showRoomsUserName(QVariantMap mapRooms)
{
    // делаем кнопку авторизации недоступной
    ui->pbAuth->setEnabled(false);
    // создаем объект QLabel и записываем имя клиента
    QLabel *lblName = new QLabel(client->getName());
    // помещаем QLabel в mainToolBar
    ui->mainToolBar->addWidget(lblName);
    //qDebug() << "mapRooms" << mapRooms;
    for (const QString& roomID: mapRooms.keys()){
        QVariantMap mapRoomName = mapRooms[roomID].toMap();
        for (const QString& roomName: mapRoomName.keys()) {
            // создаем кнопку-комнату
            RoomButton *btnRoom = new RoomButton(roomID,roomName,mapRoomName[roomName].toMap());
            // клик по комнате - показываем сообщения комнаты
            connect (btnRoom, &RoomButton::clicked, this, &MainWindow::showMessage);
            // собираем массив кнопок-комнат
            mapRoomButton[btnRoom] = btnRoom->getRoomID();
            btnRoom->setText(roomName);
            ui->vltListRooms->addWidget(btnRoom);
        }
    }
}

void MainWindow::showMessage()
{
    ui->teChat->clear();
    QString sTemp="";
    // определяем, какая кнопка нажата и считывем информацию о сообщениях в комнате
    QVariantMap mapAllMess = static_cast<RoomButton*>(sender())->getMapUserMess();
    // фиксируем ID активной комнаты
    this->roomActivID = static_cast<RoomButton*>(sender())->getRoomID().toInt();
    qDebug() << "clicked from room button, room id is " << roomActivID;
    // обрабатываем прочитанные сообщения
    QVariantMap mapReadMessage = mapAllMess["read"].toMap();
    //qDebug() << "mapReadMessage" << mapReadMessage;
    for (const QString& timeMess: mapReadMessage.keys()){
        sTemp += timeMess + ": ";
        QVariantMap mapSenderMessage = mapReadMessage[timeMess].toMap();
        for (const QString& sender: mapSenderMessage.keys()){
            ui->teChat->setAlignment(Qt::AlignCenter);
            sTemp += "from " + sender + "\n";
            ui->teChat->insertPlainText(sTemp);
            ui->teChat->setAlignment(Qt::AlignLeft);
            ui->teChat->insertPlainText(mapSenderMessage[sender].toString()+"\n");
        }
        sTemp.clear();
    }
    // обрабатываем непрочитанные сообщения
    sTemp = "\n Unread \n";
    ui->teChat->setAlignment(Qt::AlignCenter);
    ui->teChat->insertPlainText(sTemp);
    sTemp.clear();
    QVariantMap mapUnreadMessage = mapAllMess["unread"].toMap();
    //qDebug() << "mapUnreadMessage" << mapUnreadMessage;
    for (const QString& timeMess: mapUnreadMessage.keys()){
        sTemp = timeMess + ": ";
        QVariantMap mapSenderMessage = mapUnreadMessage[timeMess].toMap();
        for (const QString& sender: mapSenderMessage.keys()){
            ui->teChat->setAlignment(Qt::AlignCenter);
            sTemp += "from " + sender + "\n";
            ui->teChat->insertPlainText(sTemp);
            ui->teChat->setAlignment(Qt::AlignLeft);
            ui->teChat->insertPlainText(mapSenderMessage[sender].toString()+"\n");
        }
        sTemp.clear();
    }
    ui->pbSend->setEnabled(true);
}


void MainWindow::on_actionAuth_triggered()
{
    collectDataAuth();

}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::exit();
}
