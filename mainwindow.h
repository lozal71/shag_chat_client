#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include "chatclient.h"
#include "roombutton.h"
#include <QMainWindow>
#include <dialogauth.h>
#include <QMessageBox>
#include <QInputDialog>

QT_BEGIN_NAMESPACE
namespace Ui {class MainWindow;}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    chatClient* getClient();
private:
    Ui::MainWindow *ui;
    chatClient * client;
    int roomActivID;
    QMap<RoomButton*,QString> mapRoomButton;

    void connectClientUI();
    void showWarning(QString sParam);
    void collectDataAuth();
    void collectDataSend();
    void showRoomsUserName(QVariantMap mapRooms);
    void showMessage();
    void upgradeRooms(int roomID);
signals:
    void dataAuthCollected();
    void dataSendCollected(int roomID);
    void dataNewRoomCollected();

private slots:
    void on_actionQuit_triggered();
    void on_actionNewRoom_triggered();
};

#endif // MAINWINDOW_H
