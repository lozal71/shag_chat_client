#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QPushButton>
//#include <QLineEdit>
#include "chatclient.h"
#include "roombutton.h"

namespace Ui {
class windowServer;
}

class windowServer : public QMainWindow
{
    Q_OBJECT

public:
    explicit windowServer(QWidget *parent = nullptr);
    ~windowServer();
private:
    Ui::windowServer *ui;
    chatClient * client;
    int roomActivID;
    void connectClientUI();
    void logServerResponds(QString sParam);
    void fullCbxLogins();
    void fullCbxPasswords();
    void collectDataAuth();
    void collectDataSend();
    void showRoomsUserName(QVariantMap mapRooms);
    //void showName();
    void showMessage();

    //QMap<QPushButton*, QString> roomButtonToRoomId;
    QMap<RoomButton*, QString> mapRoomButton;
signals:
    void dataAuthCollected();
    void dataSendCollected(int roomID);
};

#endif // MAINWINDOW_H
