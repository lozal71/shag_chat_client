#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include "chatclient.h"
#include "roombutton.h"
#include <QMainWindow>
#include <dialogauth.h>
#include <QInputDialog>
#include "dialoginvite.h"
#include "notifybutton.h"

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
    DialogAuth *dialogAuth;
    DialogInvite *invite;
    QLabel *lblWarning;
    QLabel *lblUserName;
public slots:
    void resetNotifyButton(int indexInvite);
private:
    Ui::MainWindow *ui;
    chatClient * client;
    RoomButton* roomActiv;
    NotifyButton *inviteActiv;
    QList<RoomButton*> listRoomButton;
    QList<NotifyButton*> listNotifyButton;
    bool flagActionQuit;
    void connectClientUI();
    void showWarningDisconnect(QString sParam, setDisconnect discParam);
    void showWarning(QString sParam);
    void collectDataAuth();
    void collectDataSend();
    void showRoomsUserName(QVariantMap mapRoomsID);
    void showMessage();
    void showCastDelRoom(QVariantMap mapData);
    void showCast(QVariantMap mapData);
    void upgradeRoomsAdmin(QVariantMap mapNewRoom);
    void upgradeRoomsUser(QVariantMap mapNewRoom);
    void delRoom(int delRoomID);
    void showMessToTextEdit(QVariantMap mapMessID);
    void showNotifyInvite(QVariantMap mapInvitations);
    void showAcceptInvite();
signals:
    void dataAuthCollected(QString login, QString pass);
    void dataNewRoomCollected(QString newRoomName);
    void dataDelRoomCollected(int delRoomID);
private slots:
    void on_actionQuit_triggered();
    void on_actionNewRoom_triggered();
    void on_actionAuth_triggered();
    void on_actionDeleteRoom_triggered();

    void on_actionInvite_triggered();
};

#endif // MAINWINDOW_H
