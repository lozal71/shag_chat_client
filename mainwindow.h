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
    RoomButton* roomActiv;
    QList<RoomButton*> listRoomButton;
    void connectClientUI();
    void showWarning(QString sParam);
    void collectDataAuth();
    void collectDataSend();
    void showRoomsUserName(QVariantMap mapRoomsID);
    void showMessage();
    void showCastDelRoom(QVariantMap mapData);
    void showCast(QVariantMap mapData);
    void upgradeRooms(QVariantMap mapNewRoom);
    void delRoom(int delRoomID);
    void showMessToTextEdit(QVariantMap mapMessID);
signals:
    void dataAuthCollected();
    void dataNewRoomCollected(QString newRoomName);
    void dataDelRoomCollected(int delRoomID);
private slots:
    void on_actionQuit_triggered();
    void on_actionNewRoom_triggered();
    void on_actionAuth_triggered();
    void on_actionDeleteRoom_triggered();
};

#endif // MAINWINDOW_H
