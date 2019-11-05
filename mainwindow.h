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
    void connectClientUI();
    void logServerResponds(QString sParam);
    void fullCbxLogins();
    void fullCbxPasswords();
    void collectDataAuth();
    void drawRooms(QVariantMap mapRooms);
    void showMessage(QVariantMap  mapUserMess);

    //QMap<QPushButton*, QString> roomButtonToRoomId;
    QMap<RoomButton*, QString> mapRoomButton;
signals:
    void dataAuthCollected();
};

#endif // MAINWINDOW_H
