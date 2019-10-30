#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include "chatclient.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setConnect();
public slots:
    void logServerResponds(setCodeCommand code, QJsonObject joRespond);
    void slotSendQueryAuth();
private:
    Ui::MainWindow *ui;
    chatClient * clientSocket;
    void fullCbxLogins();
    void fullCbxPasswords();
signals:
    void SendQueryAuth(QJsonObject joAuth);
};

#endif // MAINWINDOW_H
