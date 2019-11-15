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
    // сервер прислал ответ на авторизацию - показываем комнаты и имя пользователя
    connect(client, &chatClient::serverRespondedAuth,
            this, &MainWindow::showRoomsUserName);
    // клик по кнопке-Send - собираем данные для отправки сообщения
    connect(ui->pbSend, &QPushButton::clicked,
            this, &MainWindow::collectDataSend);

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

// сбор данных для отправки сообщения
void MainWindow::collectDataSend()
{
    // если LineEdit не пусто
    if (!ui->leWriteMes->text().isEmpty()){
        //client->setText(ui->leWriteMes->text());
        // показваем сообщение в TextRdit справа
        ui->teChat->setAlignment(Qt::AlignRight);
        ui->teChat->insertPlainText( ui->leWriteMes->text() +"\n");

        // подготовка запроса - "отправка сообщения"
        client->prepareQuerySendMessage(roomActiv->getRoomID(),
                                        ui->leWriteMes->text());
        // очищаем LineEdit
        ui->leWriteMes->clear();
        //emit dataSendCollected(roomActiv->getRoomID());
    }
}

// показ комнат и имени пользователя
void MainWindow::showRoomsUserName(QVariantMap mapRole)
{
    ui->actionAuth->setEnabled(false);
    // создаем объект QLabel и записываем имя клиента
    QLabel *lblName = new QLabel(client->getName());
    // помещаем QLabel в mainToolBar
    ui->mainToolBar->addWidget(lblName);
    // разворачиваем map по ключу role: admin или user
    for (const QString& role: mapRole.keys()){
        QVariantMap mapRooms = mapRole[role].toMap();
        // по ключу ID - комнаты
        for (const QString& roomID: mapRooms.keys()){
            QVariantMap mapRoomName = mapRooms[roomID].toMap();
            // по имени комнаты
            for (const QString& roomName: mapRoomName.keys()) {
                // создаем кнопку-комнату
                RoomButton *btnRoom = new RoomButton(roomID.toInt(), role,
                                                     roomName,
                                                     mapRoomName[roomName].toMap());
                // клик по комнате - показ сообщения комнаты
                connect (btnRoom, &RoomButton::clicked, this, &MainWindow::showMessage);
                // собираем список кнопок-комнат
                listRoomButton.append(btnRoom);
                // добавляем виджет на вертикальный слой
                ui->vltListRooms->addWidget(btnRoom);
                // комнату с ID = 1 (default) назначаем активной
                if (btnRoom->getRoomID() == 1){
                    this->roomActiv = btnRoom;
                }
            }
        }
    }
}

// показ сообщений комнаты
void MainWindow::showMessage()
{
    ui->pbSend->setEnabled(true);
    ui->teChat->clear();
    // определяем, какая кнопка нажата и назначаем ее активной
    this->roomActiv = static_cast<RoomButton*>(sender());
    // определяем сообщения в активной комнате
    QVariantMap mapAllMess = roomActiv->getMapUserMess();
    // выделяем цветом активную комнату
    this->roomActiv->setStyleSheet("font: 14px; color: white; background-color: blue");
    // показываем прочитанные сообщения
    QVariantMap mapReadMessage = mapAllMess["read"].toMap();
    showMessToTextEdit(mapReadMessage);
    // показываем непрочитанные сообщения
    QVariantMap mapUnreadMessage = mapAllMess["unread"].toMap();
    if (!mapUnreadMessage.isEmpty()){
        ui->teChat->setAlignment(Qt::AlignCenter);
        ui->teChat->insertPlainText("\n Unread \n");
        showMessToTextEdit(mapUnreadMessage);
    }
    // показываем сообщения из онлайн рассылки
    if (!roomActiv->getListCastMess().isEmpty()){
        ui->teChat->setAlignment(Qt::AlignCenter);
        ui->teChat->insertPlainText("\n Cast \n");
        QListIterator<QVariantMap> iCast(roomActiv->getListCastMess());
        QVariantMap mapMess;
        while (iCast.hasNext()){
            mapMess = iCast.next();
            QString timeMess;
            QString senderName;
            QString text;
             timeMess =  mapMess["timeMess"].toString();
             senderName = mapMess["senderName"].toString();
             text =  mapMess["textMess"].toString();
             ui->teChat->setAlignment(Qt::AlignCenter);
             ui->teChat->insertPlainText(timeMess + ": from " + senderName + "\n");
             ui->teChat->setAlignment(Qt::AlignLeft);
             ui->teChat->insertPlainText(text + "\n");
        }
    }
    // все остальные кнопки-комнаты красим в серый цвет
    QListIterator <RoomButton*> iRoom(listRoomButton);
    //QMutableListIterator<RoomButton*> iRoom(listRoomButton);
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
    qDebug() << "showCast";
    int roomID = mapData.firstKey().toInt();
    //qDebug() << "showCast roomID " << roomID;
    QVariantMap castMapMess = mapData.first().toMap();
    QListIterator<RoomButton*> iRoom(listRoomButton);
    //QMutableListIterator<RoomButton*> iRoom(listRoomButton);
    RoomButton* currRoom = roomActiv;
    QList<QVariantMap> castList;
    while (iRoom.hasNext()){
        currRoom = iRoom.next();
        if (currRoom->getRoomID() == roomID ) {
            qDebug() << "showCast 226";
            castList = currRoom->getListCastMess();
            castList.append(castMapMess);
            break;
        }
    }
    currRoom->setListCastMess(castList);
    if (currRoom != roomActiv){
        currRoom->setStyleSheet("font: 14px; color: white; background-color: green");
    }
    else{
        upgradeMessage();
    }
    qDebug() << "232" << currRoom->getListCastMess();
}

void MainWindow::upgradeRooms(QVariantMap mapNewRoom)
{
    QVariantMap nullMapMess;
    // создаем кнопку-комнату
    RoomButton *btnRoom = new RoomButton(mapNewRoom["newRoomID"].toInt(), "admin",
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

void MainWindow::showMessToTextEdit(QVariantMap mapMessID)
{
    QString timeMess;
    QString senderName;
    QString text;
    for (const QString& messID: mapMessID.keys()){
        QVariantMap mapMess = mapMessID[messID].toMap();
         timeMess =  mapMess["timeMess"].toString();
         senderName = mapMess["senderName"].toString();
         text =  mapMess["textMess"].toString();
         ui->teChat->setAlignment(Qt::AlignCenter);
         ui->teChat->insertPlainText(timeMess + ": from " + senderName + "\n");
         ui->teChat->setAlignment(Qt::AlignLeft);
         ui->teChat->insertPlainText(text + "\n");
    }
}

void MainWindow::upgradeMessage()
{
    ui->teChat->clear();
    // определяем сообщения в активной комнате
    QVariantMap mapAllMess = roomActiv->getMapUserMess();
     // показываем прочитанные сообщения
    QVariantMap mapReadMessage = mapAllMess["read"].toMap();
    showMessToTextEdit(mapReadMessage);
    // показываем непрочитанные сообщения
    QVariantMap mapUnreadMessage = mapAllMess["unread"].toMap();
    if (!mapUnreadMessage.isEmpty()){
        ui->teChat->setAlignment(Qt::AlignCenter);
        ui->teChat->insertPlainText("\n Unread \n");
        showMessToTextEdit(mapUnreadMessage);
    }
    // показываем сообщения из онлайн рассылки
    if (!roomActiv->getListCastMess().isEmpty()){
        ui->teChat->setAlignment(Qt::AlignCenter);
        ui->teChat->insertPlainText("\n Cast \n");
        QListIterator<QVariantMap> iCast(roomActiv->getListCastMess());
        QVariantMap mapMess;
        while (iCast.hasNext()){
            mapMess = iCast.next();
            QString timeMess;
            QString senderName;
            QString text;
             timeMess =  mapMess["timeMess"].toString();
             senderName = mapMess["senderName"].toString();
             text =  mapMess["textMess"].toString();
             ui->teChat->setAlignment(Qt::AlignCenter);
             ui->teChat->insertPlainText(timeMess + ": from " + senderName + "\n");
             ui->teChat->setAlignment(Qt::AlignLeft);
             ui->teChat->insertPlainText(text + "\n");
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
        //QListIterator<RoomButton*> iRoom(listRoomButton);
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
