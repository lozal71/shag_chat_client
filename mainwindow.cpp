#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client = new chatClient();
    connectClientUI();
    ui->leWriteMes->setPlaceholderText("Please, write here...");
    ui->teChat->setReadOnly(true);
}

MainWindow::~MainWindow()
{
    delete client;
    delete ui;
}

chatClient* MainWindow::getClient()
{
    return client;
}


void MainWindow::connectClientUI()
{
    // данные для Авторизации собраны - клиент готовит запрос на Авторизацию
    connect(this,&MainWindow::dataAuthCollected,
            client, &chatClient::prepareQueryAuth);
    // сессия закрылась - выводим сообщение в окно логирования
    connect(client, &chatClient::sessionClosed,
            this, &MainWindow::showWarning);
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

    connect(this,&MainWindow::dataNewRoomCollected,
            client, &chatClient::prepareQueryNewRoom);

    connect(client, &chatClient::serverRaspondedNewRoom,
            this, &MainWindow::upgradeRooms);

    connect(this,&MainWindow::dataDelRoomCollected,
            client, &chatClient::prepareQueryDelRoom);
 }


void MainWindow::showWarning(QString sParam)
{
    QMessageBox msbx(QMessageBox::Warning,"Warning",sParam);
    msbx.exec();
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
        emit dataSendCollected(roomActiv->getRoomID());
    }
}

void MainWindow::showRoomsUserName(QVariantMap mapRooms)
{
    // создаем объект QLabel и записываем имя клиента
    QLabel *lblName = new QLabel(client->getName());
    // помещаем QLabel в mainToolBar
    ui->mainToolBar->addWidget(lblName);
    //qDebug() << "mapRooms" << mapRooms;
    for (const QString& roomID: mapRooms.keys()){
        QVariantMap mapRoomName = mapRooms[roomID].toMap();
        for (const QString& roomName: mapRoomName.keys()) {
            // создаем кнопку-комнату
            RoomButton *btnRoom = new RoomButton(roomID.toInt(),
                                                 roomName,
                                                 mapRoomName[roomName].toMap());
            // клик по комнате - показываем сообщения комнаты
            connect (btnRoom, &RoomButton::clicked, this, &MainWindow::showMessage);
            // собираем список кнопок-комнат
            listRoomButton.append(btnRoom);
            ui->vltListRooms->addWidget(btnRoom);
        }
    }
}

void MainWindow::showMessage()
{
    ui->teChat->clear();
    QString sTemp="";
    // определяем, какая кнопка нажата и считывем информацию о сообщениях в комнате
    this->roomActiv = static_cast<RoomButton*>(sender());
    QVariantMap mapAllMess = roomActiv->getMapUserMess();
    // выделяем цветом активную комнату
    this->roomActiv->setStyleSheet("font: 14px; color: white; background-color: blue");
    qDebug() << "clicked from room button, room id is " << roomActiv->getRoomID();
    // показываем прочитанные сообщения
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
    // показываем непрочитанные сообщения
    sTemp = "\n Unread \n";
    ui->teChat->setAlignment(Qt::AlignCenter);
    ui->teChat->insertPlainText(sTemp);
    sTemp.clear();
    ui->teChat->insertPlainText(sTemp);
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

    QMutableListIterator<RoomButton*> iRoom(listRoomButton);
    RoomButton* currRoom;
    while (iRoom.hasNext()){
        currRoom = iRoom.next();
        if (currRoom->getRoomID() != roomActiv->getRoomID()){
            currRoom->setStyleSheet("font: 14px; color: black; background-color: gray");
        }
    }

}

void MainWindow::upgradeRooms(QVariantMap mapNewRoom)
{
    QVariantMap nullMapMess;
    // создаем кнопку-комнату
    RoomButton *btnRoom = new RoomButton(mapNewRoom["newRoomID"].toInt(),
                                         mapNewRoom["newRoomName"].toString(),
                                         nullMapMess);
    // клик по комнате - показываем сообщения комнаты
    connect (btnRoom, &RoomButton::clicked, this, &MainWindow::showMessage);
    listRoomButton.append(btnRoom);
    QMutableListIterator<RoomButton*> iRoom(listRoomButton);
    RoomButton* currRoom;
    while (iRoom.hasNext()){
        currRoom = iRoom.next();
        ui->vltListRooms->addWidget(currRoom);
    }
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_actionNewRoom_triggered()
{
    bool ok;
    QString roomName = QInputDialog::getText(this,"New Room",
                                             "Name of room",
                                             QLineEdit::Normal, "Input name here", &ok);
    if (ok && !roomName.isEmpty()){
        emit dataNewRoomCollected(roomName);
        qDebug() << "roomName" << roomName;
    }

}

void MainWindow::on_actionAuth_triggered()
{
    DialogAuth *uiLog = new DialogAuth();
    if(uiLog->exec() == QDialog::Accepted){
        client->setLogin(uiLog->getLogin());
        client->setPass(uiLog->getPass());
        emit dataAuthCollected();
    }
}

void MainWindow::on_actionDeleteRoom_triggered()
{
    QString sTemp = "Delete room " + roomActiv->getRoomName() + " ?";
    QMessageBox quest(QMessageBox::Question,"Question",sTemp);
    if(quest.exec()) {
        emit dataDelRoomCollected(roomActiv->getRoomID());
    }
}
