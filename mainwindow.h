#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include "chatclient.h"
#include "roombutton.h"
#include <QMainWindow>
#include <dialogauth.h>

QT_BEGIN_NAMESPACE
namespace Ui {class MainWindow;}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    Ui::MainWindow *ui;
    DialogAuth *uiLog;
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
    QMap<RoomButton*,QString> mapRoomButton;
signals:
    void dataAuthCollected();
    void dataSendCollected(int roomID);

private slots:
    void on_actionAuth_triggered();
    void on_actionQuit_triggered();
};

#endif // MAINWINDOW_H
