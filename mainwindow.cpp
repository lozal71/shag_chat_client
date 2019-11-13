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
    ui->actionAuth->setEnabled(false);
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
    connect(client, &chatClient::noConnect,
            this,&MainWindow::showWarning);
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

    connect(client, &chatClient::serverCastDelRoom,
            this, &MainWindow::showCastDelRoom);

    connect(client, &chatClient::serverCast,
            this, &MainWindow::showCast);

    connect(client, &chatClient::serverDeletedRoom,
            this, &MainWindow::delRoom);
 }


void MainWindow::showWarning(QString sParam)
{
    QMessageBox msbx(QMessageBox::Warning,"Warning",sParam);
    msbx.exec();
    ui->actionAuth->setEnabled(true);
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

void MainWindow::showRoomsUserName(QVariantMap mapRole)
{
    ui->actionAuth->setEnabled(false);
    // создаем объект QLabel и записываем имя клиента
    QLabel *lblName = new QLabel(client->getName());
    // помещаем QLabel в mainToolBar
    ui->mainToolBar->addWidget(lblName);
    //qDebug() << "mapRooms" << mapRooms;
    for (const QString& role: mapRole.keys()){
        QVariantMap mapRooms = mapRole[role].toMap();
        for (const QString& roomID: mapRooms.keys()){
            QVariantMap mapRoomName = mapRooms[roomID].toMap();
            for (const QString& roomName: mapRoomName.keys()) {
                // создаем кнопку-комнату
                if (role == "admin"){
                    RoomButton *btnRoom = new RoomButton(roomID.toInt(), 1,
                                                         roomName,
                                                         mapRoomName[roomName].toMap());
                    // клик по комнате - показываем сообщения комнаты
                    connect (btnRoom, &RoomButton::clicked, this, &MainWindow::showMessage);
                    // собираем список кнопок-комнат
                    listRoomButton.append(btnRoom);
                    ui->vltListRooms->addWidget(btnRoom);
                }
                else{
                    RoomButton *btnRoom = new RoomButton(roomID.toInt(), 2,
                                                         roomName,
                                                         mapRoomName[roomName].toMap());
                    // клик по комнате - показываем сообщения комнаты
                    connect (btnRoom, &RoomButton::clicked, this, &MainWindow::showMessage);
                    // собираем список кнопок-комнат
                    listRoomButton.append(btnRoom);
                    ui->vltListRooms->addWidget(btnRoom);
                    if (btnRoom->getRoomID() == 1){
                        this->roomActiv = btnRoom;
                        this->roomActiv->setStyleSheet("font: 14px; color: white; background-color: blue");
                    }
                }

            }
        }
    }
}

void MainWindow::showMessage()
{
    ui->pbSend->setEnabled(true);
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
    QVariantMap mapUnreadMessage = mapAllMess["unread"].toMap();
    if (!mapUnreadMessage.isEmpty()){
        sTemp = "\n Unread \n";
        ui->teChat->setAlignment(Qt::AlignCenter);
        ui->teChat->insertPlainText(sTemp);
        sTemp.clear();
        ui->teChat->insertPlainText(sTemp);

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
            mapReadMessage[timeMess] = mapSenderMessage;
            sTemp.clear();
        }
    }
    mapUnreadMessage.clear();
    QVariantMap mapCastMessage = mapAllMess["cast"].toMap();
    if (!mapCastMessage.isEmpty()){
        // показываем рассылку
        sTemp = "\n Cast \n";
        ui->teChat->setAlignment(Qt::AlignCenter);
        ui->teChat->insertPlainText(sTemp);
        sTemp.clear();
        ui->teChat->insertPlainText(sTemp);

        //qDebug() << "mapUnreadMessage" << mapUnreadMessage;
        for (const QString& timeMess: mapCastMessage.keys()){
            sTemp = timeMess + ": ";
            QVariantMap mapSenderMessage = mapCastMessage[timeMess].toMap();
            for (const QString& sender: mapSenderMessage.keys()){
                ui->teChat->setAlignment(Qt::AlignCenter);
                sTemp += "from " + sender + "\n";
                ui->teChat->insertPlainText(sTemp);
                ui->teChat->setAlignment(Qt::AlignLeft);
                ui->teChat->insertPlainText(mapSenderMessage[sender].toString()+"\n");
            }
            mapReadMessage[timeMess] = mapSenderMessage;
            sTemp.clear();
        }
    }
    mapCastMessage.clear();
    QMutableListIterator<RoomButton*> iRoom(listRoomButton);
    RoomButton* currRoom;
    while (iRoom.hasNext()){
        currRoom = iRoom.next();
        if (currRoom->getRoomID() != roomActiv->getRoomID()){
            currRoom->setStyleSheet("font: 14px; color: black; background-color: gray");
        }
    }

}

void MainWindow::showCastDelRoom(QVariantMap mapData)
{
    //qDebug() << "showCastDelRoom";
    QMutableListIterator<RoomButton*> iRoom(listRoomButton);
    RoomButton* currRoom;
    while (iRoom.hasNext()){
        currRoom = iRoom.next();
        if (currRoom->getRoomID() == 1 ) {
            this->roomActiv = currRoom;
            this->roomActiv->setStyleSheet("font: 14px; color: white; background-color: blue");
            QString sTemp;
            ui->teChat->setAlignment(Qt::AlignCenter);
            sTemp = "from server cast \n";
            ui->teChat->insertPlainText(sTemp);
            ui->teChat->setAlignment(Qt::AlignLeft);
            ui->teChat->insertPlainText(mapData["cast"].toString());
            break;
        }
    }
    QMutableListIterator<RoomButton*> iiRoom(listRoomButton);
    iRoom.toFront();
    while (iiRoom.hasNext()){
         currRoom = iiRoom.next();
         //qDebug() << currRoom->getRoomName();
        if (currRoom->getRoomID() == mapData["roomID"].toInt() ){
            //qDebug() << "remove room ";
            ui->vltListRooms->removeWidget(currRoom);
            ui->vltListRooms->update();
            //qDebug() << "listRoomButton" << listRoomButton;
            iiRoom.remove();
            delete currRoom;
            //qDebug() << "listRoomButton" << listRoomButton;
            break;
        }
    }

    iRoom.toFront();
    while (iRoom.hasNext()){
        currRoom = iRoom.next();
        if (currRoom->getRoomID() != 1 ) {
            currRoom->setStyleSheet("font: 14px; color: black; background-color: gray");
        }
    }
}

void MainWindow::showCast(QVariantMap mapData)
{
    int roomID = mapData["roomID"].toInt();
    QVariantMap newMapMess;
    QMutableListIterator<RoomButton*> iRoom(listRoomButton);
    RoomButton* currRoom;
    while (iRoom.hasNext()){
        currRoom = iRoom.next();
        if (currRoom->getRoomID() == roomID ) {
            currRoom->setStyleSheet("font: 14px; color: white; background-color: green");
            newMapMess = currRoom->getMapUserMess();
            newMapMess["cast"] = mapData["cast"].toMap();
            currRoom->setMapMess(newMapMess);
            //qDebug() << "277" << currRoom->getMapUserMess();
            break;
        }
    }
}

void MainWindow::upgradeRooms(QVariantMap mapNewRoom)
{
    QVariantMap nullMapMess;
    // создаем кнопку-комнату
    RoomButton *btnRoom = new RoomButton(mapNewRoom["newRoomID"].toInt(), 1,
                                         mapNewRoom["newRoomName"].toString(),
                                         nullMapMess);
    // клик по комнате - показываем сообщения комнаты
    connect (btnRoom, &RoomButton::clicked, this, &MainWindow::showMessage);
    listRoomButton.append(btnRoom);
    ui->vltListRooms->addWidget(btnRoom);
    ui->vltListRooms->update();
}

void MainWindow::delRoom(int delRoomID)
{
    QMutableListIterator<RoomButton*> iRoom(listRoomButton);
    RoomButton* currRoom;
    // находим комнату с id = 1 и делаем активной
    while (iRoom.hasNext()){
        currRoom = iRoom.next();
        if (currRoom->getRoomID() == 1){
            roomActiv = currRoom;
            this->roomActiv->setStyleSheet("font: 14px; color: white; background-color: blue");
            break;
        }
    }
    // находим комнату с id=delRoomID и удаляем ее
    iRoom.toFront();
    while (iRoom.hasNext()){
        currRoom = iRoom.next();
        if (currRoom->getRoomID() == delRoomID) {
            ui->vltListRooms->removeWidget(currRoom);
            ui->vltListRooms->update();
           // qDebug() << "listRoomButton" << listRoomButton;
            iRoom.remove();
            delete currRoom;
           // qDebug() << "listRoomButton" << listRoomButton;
            break;
        }
    }

    iRoom.toFront();
    while (iRoom.hasNext()){
        currRoom = iRoom.next();
        if (currRoom->getRoomID() != 1){
            currRoom->setStyleSheet("font: 14px; color: black; background-color: gray");
        }
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
        QMutableListIterator<RoomButton*> iRoom(listRoomButton);
        RoomButton* currRoom;
        while (iRoom.hasNext()){
            currRoom = iRoom.next();
            ui->vltListRooms->removeWidget(currRoom);
            iRoom.remove();
           delete currRoom;
        }
        ui->mainToolBar->clear();
        client->setLogin(uiLog->getLogin());
        client->setPass(uiLog->getPass());
        emit dataAuthCollected();
    }
}

void MainWindow::on_actionDeleteRoom_triggered()
{
    int r;
    QString sTemp = "Delete room " + roomActiv->getRoomName() + " ?";
    if (roomActiv->getRole() == 1){
        QMessageBox quest(QMessageBox::Question,"Question",sTemp);
        quest.setStandardButtons(QMessageBox::Yes| QMessageBox::No);
        r=quest.exec();
        if(r == QMessageBox::Yes) {
            emit dataDelRoomCollected(roomActiv->getRoomID());
        }
    }
    else{
        showWarning("You are not admin of room: " + roomActiv->getRoomName());
    }
}
