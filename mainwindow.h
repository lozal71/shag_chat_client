#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
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
    //void slotSendQueryAuth();
private:
    Ui::MainWindow *ui;
    chatClient * client;
    void fullCbxLogins();
    void fullCbxPasswords();
    void collectDataAuth();
signals:
    void dataAuthCollected();
};

#endif // MAINWINDOW_H
