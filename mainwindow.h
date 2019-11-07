#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QPushButton>
#include "chatclient.h"
#include "roombutton.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    chatClient * client;
    int roomActivID;
    void connectClientUI();
    void logServerResponds(QString sParam);
    void fullCbxLogins();
    void fullCbxPasswords();
    void collectDataAuth();
    void collectDataSend();
    void showRooms(QVariantMap mapRooms);
    void showName();
    void showMessage();

    //QMap<QPushButton*, QString> roomButtonToRoomId;
    QMap<RoomButton*, QString> mapRoomButton;
signals:
    void dataAuthCollected();
    void dataSendCollected(int roomID);
};

#endif // MAINWINDOW_H
