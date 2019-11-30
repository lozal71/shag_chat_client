#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client = new chatClient();
    dialogAuth = new DialogAuth();
    invite = new DialogInvite();
    lblWarning = new QLabel();
    lblUserName = new QLabel();
    inviteActiv = new NotifyButton();
    connectClientUI();
    ui->leWriteMes->setPlaceholderText("Please, write here...");
    ui->teChat->setReadOnly(true);
    ui->actionAuth->setEnabled(false);
    ui->leWriteMes->setEnabled(false);
    flagActionQuit = false;
    ui->vltNotify->addWidget(inviteActiv);
}

MainWindow::~MainWindow()
{
    if (!listRoomButton.isEmpty()){
        QMutableListIterator<RoomButton*> iRoom(listRoomButton);
        RoomButton* currRoom;
        while (iRoom.hasNext()){
            currRoom = iRoom.next();
            iRoom.remove();
            delete currRoom;
        }
    }
    if (!listNotifyButton.isEmpty()){
        QMutableListIterator<NotifyButton*> iNotify(listNotifyButton);
        NotifyButton* currNotify;
        while(iNotify.hasNext()){
            currNotify = iNotify.next();
            iNotify.remove();
            delete currNotify;
        }
    }
    if (!inviteActiv) delete inviteActiv;
    if (!lblWarning) delete lblWarning;
    if (!lblUserName) delete lblUserName;
    if (!invite) delete invite;
    delete dialogAuth;
    delete client;
    delete ui;
}

chatClient* MainWindow::getClient()
{
    return client;
}

void MainWindow::resetNotifyButton(int indexInvite)
{
    //qDebug() << "indexInvite" << indexInvite;
    inviteActiv->setEnabled(true);
    QListIterator<NotifyButton*> iNotify(listNotifyButton);
    NotifyButton *currNotify = inviteActiv;
    while (iNotify.hasNext()){
        currNotify = iNotify.next();
        if (currNotify->getIndex()==indexInvite){
            inviteActiv->setParam(currNotify->getIndex(),
                                  currNotify->getMap(),
                                  currNotify->getInviteID());
            //qDebug() << "inviteActiv" << inviteActiv;
            //qDebug() << "inviteActiv->getMap()" << inviteActiv->getMap();
            break;
        }
    }
}

void MainWindow::connectClientUI()
{
    // данные для Авторизации собраны - клиент готовит запрос на Авторизацию
    connect(this,&MainWindow::dataAuthCollected,
            client, &chatClient::prepareQueryAuth);

    // сессия закрылась - выводим предупреждение
    connect(client, &chatClient::sessionClosed,
            this, &MainWindow::showWarningDisconnect);

    connect(client, &chatClient::noConnect,
            this,&MainWindow::showWarningDisconnect);

    connect(client, &chatClient::authNotCorrected,
            dialogAuth, &DialogAuth::showWarningAuth);

    // успешная авторизация  - показываем комнаты и имя пользователя
    connect(client, &chatClient::authSuccess,
            this, &MainWindow::showRoomsUserName);

    // клик по кнопке-Send - собираем данные для отправки сообщения
    connect(ui->pbSend, &QPushButton::clicked,
            this, &MainWindow::collectDataSend);
    connect(ui->leWriteMes, &QLineEdit::returnPressed,
            this, &MainWindow::collectDataSend);

    connect(this,&MainWindow::dataNewRoomCollected,
            client, &chatClient::prepareQueryNewRoom);

    connect(client, &chatClient::serverRaspondedNewRoom,
            this, &MainWindow::upgradeRoomsAdmin);

    connect(this,&MainWindow::dataDelRoomCollected,
            client, &chatClient::prepareQueryDelRoom);

    connect(client, &chatClient::serverCastDelRoom,
            this, &MainWindow::showCastDelRoom);

    connect(client, &chatClient::serverCast,
            this, &MainWindow::showCast);

    connect(client, &chatClient::serverDeletedRoom,
            this, &MainWindow::delRoom);

    connect(client, &chatClient::readInvite,
            this, &MainWindow::showNotifyInvite);

    connect(ui->cbxNotify, SIGNAL(activated(int)),
            this, SLOT(resetNotifyButton(int)));

    connect(inviteActiv, &NotifyButton::clicked,
            this, &MainWindow::showAcceptInvite);

    connect(client, &chatClient::RoomsUserUpgrate,
            this, &MainWindow::upgradeRoomsUser);

    connect(client, &chatClient::notifyUpgrated,
            this, &MainWindow::notifyUpgrade);

    connect(client, &chatClient::showResultInvite,
            this, &MainWindow::showWarning);
    connect(client, &chatClient::usersUpdated,
            this, &MainWindow::searchRoomButton);
}

void MainWindow::showWarning(QString sParam)
{
    QMessageBox msbx(QMessageBox::Warning,"Warning",sParam);
    msbx.exec();
}


void MainWindow::showWarningDisconnect(QString sParam, setDisconnect discParam)
{

    switch (discParam) {
    case setDisconnect::fromServer:
    {
        QMessageBox msbx(QMessageBox::Warning,"Warning",sParam);
        msbx.exec();
        ui->actionAuth->setEnabled(true);
        break;
    }
    case setDisconnect::fromClient:
        QApplication::exit(0);
        break;
     case setDisconnect::undefined:
        if (!flagActionQuit){
            QMessageBox msbx(QMessageBox::Warning,"Warning",sParam);
            msbx.exec();
        }
        ui->actionAuth->setEnabled(true);
        break;
    }
    lblWarning->setText(sParam);
    // помещаем QLabel в mainToolBar
    ui->mainToolBar->addWidget(lblWarning);
}

// сбор данных для отправки сообщения
void MainWindow::collectDataSend()
{
    if (roomActiv->getRole() == "user" && roomActiv->getMapUsers().isEmpty()){
        showWarning("Вы удалены из этой комнаты");
    }
    else {
        // если LineEdit не пусто
        if (!ui->leWriteMes->text().isEmpty()){

            // подготовка запроса - "отправка сообщения"
            client->prepareQuerySendMessage(roomActiv->getRoomID(),
                                            ui->leWriteMes->text());
            // очищаем LineEdit
            ui->leWriteMes->clear();
        }
    }
}

// показ комнат и имени пользователя
void MainWindow::showRoomsUserName(QVariantMap mapRoomsID)
{
    // авторизация прошла успешно - делаем недоступным меню авторизации
    dialogAuth->authAgain=false;
    ui->actionAuth->setEnabled(false);
    // в WindowTitle пишем имя пользователя
    qDebug() << client->getName();
    this->setWindowTitle(client->getName());
    //lblUserName->setText(client->getName() + " ");
    //ui->mainToolBar->addWidget(lblUserName);
    //ui->mainToolBar->update();
    // разворачиваем кнопки-комнаты по ключу roomID
    for (const QString& sRoomID: mapRoomsID.keys()){
        QVariantMap mapRooms = mapRoomsID[sRoomID].toMap();
        // создаем кнопку-комнату
        RoomButton *btnRoom = new RoomButton(mapRooms["roomID"].toInt(),
                                             mapRooms["role"].toString(),
                                             mapRooms["roomName"].toString(),
                                             mapRooms["mess"].toMap(),
                                             mapRooms["members"].toMap());
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

// показ сообщений комнаты
void MainWindow::showMessage()
{
    ui->pbSend->setEnabled(true);
    ui->leWriteMes->setEnabled(true);
    ui->teChat->clear();
    // определяем, какая кнопка нажата и назначаем ее активной
    this->roomActiv = static_cast<RoomButton*>(sender());
    // определяем сообщения в активной комнате
    QVariantMap mapAllMess = roomActiv->getMapMess();
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
    QMutableListIterator<RoomButton*> iiRoom(listRoomButton);
    RoomButton* currRoom = roomActiv;
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
    QList<QVariantMap> castList;
    QListIterator<RoomButton*> iRoom(listRoomButton);
    while (iRoom.hasNext()){
        currRoom = iRoom.next();
        if (currRoom->getRoomID() == 1) {
            castList = currRoom->getListCastMess();
            castList.append(mapData);
            currRoom->setStyleSheet("font: 14px; color: white; background-color: green");
            break;
        }
    }
    currRoom->setListCastMess(castList);
    QLabel *lblDelRoom = new QLabel(mapData["textMess"].toString() + " ");
    // помещаем QLabel в mainToolBar
    ui->mainToolBar->addWidget(lblDelRoom);
}

void MainWindow::showCast(QVariantMap mapData)
{
    qDebug() << "showCast";
    int roomID = mapData["roomID"].toInt();
    QListIterator<RoomButton*> iRoom(listRoomButton);
    RoomButton* currRoom = roomActiv;
    QList<QVariantMap> castList;
    while (iRoom.hasNext()){
        currRoom = iRoom.next();
        if (currRoom->getRoomID() == roomID ) {
            castList = currRoom->getListCastMess();
            castList.append(mapData);
            break;
        }
    }
    currRoom->setListCastMess(castList);
    if (currRoom != roomActiv){
        currRoom->setStyleSheet("font: 14px; color: white; background-color: green");
    }
    else{
        emit roomActiv->clicked();
        //upgradeMessage();
    }
    //qDebug() << "232" << currRoom->getListCastMess();
}

void MainWindow::upgradeRoomsAdmin(QVariantMap mapNewRoom)
{
    QVariantMap nullMapMess;
    QVariantMap nullMapUsers;
    // создаем кнопку-комнату
    RoomButton *btnRoom = new RoomButton(mapNewRoom["newRoomID"].toInt(), "admin",
                                         mapNewRoom["newRoomName"].toString(),
                                         nullMapMess, nullMapUsers);
    // клик по комнате - показываем сообщения комнаты
    connect (btnRoom, &RoomButton::clicked, this, &MainWindow::showMessage);
    listRoomButton.append(btnRoom);
    ui->vltListRooms->addWidget(btnRoom);
    ui->vltListRooms->update();
}

void MainWindow::upgradeRoomsUser(QVariantMap mapNewRoom)
{
    // создаем кнопку-комнату
    RoomButton *btnRoom = new RoomButton(mapNewRoom["roomID"].toInt(), "user",
                                         mapNewRoom["roomName"].toString(),
                                         mapNewRoom["mess"].toMap(),
                                         mapNewRoom["members"].toMap());
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
            //this->roomActiv->setStyleSheet("font: 14px; color: white; background-color: blue");
            break;
        }
    }
    // находим комнату с id = delRoomID и удаляем ее
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

void MainWindow::showNotifyInvite(QVariantMap mapInvitations)
{
    ui->lblNotify->setStyleSheet("font: 14px; color: white; background-color: green");
    for (const QString& sID: mapInvitations.keys()) {
        QVariantMap mapInvite = mapInvitations[sID].toMap();
        ui->cbxNotify->addItem("from " + mapInvite["senderName"].toString());
        NotifyButton* btnInvite = new NotifyButton(ui->cbxNotify->currentIndex(),
                                                   mapInvite, sID.toInt());
        listNotifyButton.append(btnInvite);
        //qDebug() << "listNotifyButton" << listNotifyButton;

     }

}

void MainWindow::showAcceptInvite()
{
    qDebug() << "showAcceptInvite";
    int r;
    QString sParam;
    QString senderName = inviteActiv->getMap()["senderName"].toString();
    QString roomName = inviteActiv->getMap()["roomName"].toString();
    int roomID = inviteActiv->getMap()["roomID"].toInt();
    int senderID = inviteActiv->getMap()["senderID"].toInt();
    sParam = senderName + " invite you in  " + roomName +
            +": " +inviteActiv->getMap()["text"].toString() + " Accept?" ;
    QMessageBox quest(QMessageBox::Question,"Question",sParam);
    quest.setStandardButtons(QMessageBox::Yes| QMessageBox::No);
    r=quest.exec();
    //qDebug() << "r= " << r;
    if(r == QMessageBox::Yes) {
        //qDebug() << "459 accept invite";
        ui->cbxNotify->removeItem(inviteActiv->getIndex());
        client->prepareQueryAcceptInvite(inviteActiv->getInviteID(),
                                         inviteActiv->getMap()["roomID"].toInt(),
                                         inviteActiv->getMap()["roomName"].toString());

    }
    else {
        //qDebug() << "465 reject invite";
        ui->cbxNotify->removeItem(inviteActiv->getIndex());
        client->prepareQueryRejectInvite(inviteActiv->getInviteID(),
                                         roomID, roomName,senderID,senderName);

    }
}

void MainWindow::notifyUpgrade(int inviteID)
{
    QMutableListIterator<NotifyButton*> iNotify(listNotifyButton);
    NotifyButton* currNotify;
    while(iNotify.hasNext()){
        currNotify = iNotify.next();
        if (currNotify->getInviteID() == inviteID){
            iNotify.remove();
            ui->cbxNotify->removeItem(currNotify->getIndex());
            delete currNotify;
            break;
        }
    }
    if (listNotifyButton.isEmpty()){
        inviteActiv->setNull();
        ui->lblNotify->setStyleSheet("font: 14px; color: white; background-color: gray");

    }
}

void MainWindow::showDialogMembers(QVariantMap mapUsers, int roomID)
{
    QStringList users;
    users.clear();
    for (const QString& sUserID: mapUsers.keys()) {
        users.append(mapUsers[sUserID].toString());
    }
    invite->fullCbxUserName(users);
    QStringList textList;
    textList.clear();
    textList << "text1" << "text2" << "text3";
    invite->fullCbxTextInvite(textList);
    int userID = 0;
    QString text = invite->getTextInvite();
    invite->setModal(false);
    if(invite->exec() == QDialog::Accepted){
        for (const QString& sUserID: mapUsers.keys()) {
            if (mapUsers[sUserID].toString() == invite->getUserName()){
               userID =  sUserID.toInt();
               break;
            }
        }
        client->prepareQueryDelUser(userID, roomID, text);
    }
}

void MainWindow::searchRoomButton(QVariantMap mapData)
{
    //qDebug() << "512 senderName" << mapData["senderName"].toString();
    //qDebug() << "roomID" << mapData["roomID"].toInt();
    QVariantMap currentMapUsers;
    QListIterator<RoomButton*> iRoom(listRoomButton);
    RoomButton* currentRoom = roomActiv;
    setUpdateUsers updateParam = setUpdateUsers(mapData["updateParam"].toInt());
    while (iRoom.hasNext()) {
        currentRoom = iRoom.next();
        currentMapUsers = currentRoom->getMapUsers();
        if (currentRoom->getRoomID() == mapData["roomID"].toInt()){
            if ( updateParam == setUpdateUsers::addUser){
                currentMapUsers.insert(mapData["userID"].toString(),
                                       mapData["senderName"].toString());
            }
            else {
                if (updateParam == setUpdateUsers::delIsYou){
                    currentMapUsers.clear();
                    currentRoom->setStyleSheet("font: 14px; color: black; background-color: yellow");
                }
                else {
                    currentMapUsers.remove(mapData["userID"].toString());
                }
            }
            showCast(mapData);
            break;
        }
    }
    currentRoom->setMapUsers(currentMapUsers);
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::exit();
    flagActionQuit = true;
}

void MainWindow::on_actionNewRoom_triggered()
{
    bool ok;
    QString roomName = QInputDialog::getText(this,"New Room",
                                             "Name of room",
                                             QLineEdit::Normal, "Input name here", &ok);
    if (ok && !roomName.isEmpty()){
        emit dataNewRoomCollected(roomName);
        //qDebug() << "roomName" << roomName;
    }

}

void MainWindow::on_actionAuth_triggered()
{
    QMutableListIterator<RoomButton*> iRoom(listRoomButton);
    RoomButton* currRoom;
    while (iRoom.hasNext()){
        currRoom = iRoom.next();
        ui->vltListRooms->removeWidget(currRoom);
        iRoom.remove();
       delete currRoom;
    }
    ui->mainToolBar->clear();
    dialogAuth->authAgain=false;
    dialogAuth->flagExit = false;
    do{
        if(dialogAuth->exec() == QDialog::Accepted){
            emit dataAuthCollected(dialogAuth->getLogin(),
                                   dialogAuth->getPass());
        }
    } while (dialogAuth->authAgain);
    if (dialogAuth->flagExit) {
        QApplication::exit(0);
    }
}

void MainWindow::on_actionDeleteRoom_triggered()
{
    int r;
    QString sTemp = "Delete room " + roomActiv->getRoomName() + " ?";
    if (roomActiv->getRole() == "admin"){
        QMessageBox quest(QMessageBox::Question,"Question",sTemp);
        quest.setStandardButtons(QMessageBox::Yes| QMessageBox::No);
        r=quest.exec();
        if(r == QMessageBox::Yes) {
            client->prepareQueryDelRoom(roomActiv->getRoomID());
            //emit dataDelRoomCollected(roomActiv->getRoomID());
        }
    }
    else{
        showWarning("You are not admin of room: " + roomActiv->getRoomName());
    }
}

void MainWindow::on_actionInvite_triggered()
{
    if (roomActiv->getRole() == "admin"){
        QStringList users;
        users.clear();
        users << "user1" << "user2" << "user3";
        invite->fullCbxUserName(users);
        QStringList text;
        text.clear();
        text << "text1" << "text2" << "text3";
        invite->fullCbxTextInvite(text);
        if(invite->exec() == QDialog::Accepted){
            if (client->getName() == invite->getUserName()){
                showWarning("Нельзя приглашать самого себя!!! ");
            }
            else {
                client->prepareQueryInvite(invite->getUserName(),
                                           invite->getTextInvite(),
                                           this->roomActiv->getRoomID());
            }
        }
    }
    else{
        showWarning("You are not admin of room: " + roomActiv->getRoomName());
    }
}

void MainWindow::on_actionDelete_user_triggered()
{
    if (roomActiv->getRole() == "admin"){
        showDialogMembers(roomActiv->getMapUsers(), roomActiv->getRoomID());
        //client->prepareQueryDelUser(roomActiv->getRoomID());
        //client->prepareQueryUserInRoom(roomActiv->getRoomID());
    }
    else{
        showWarning("You are not admin of room: " + roomActiv->getRoomName());
    }
}

void MainWindow::on_actionView_members_triggered()
{
    showDialogMembers(roomActiv->getMapUsers(), roomActiv->getRoomID());
}
