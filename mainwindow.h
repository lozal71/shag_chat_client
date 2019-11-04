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
private:
    Ui::MainWindow *ui;
    chatClient * client;
    void connectClientUI();
    void logServerResponds(QString sParam);
    void fullCbxLogins();
    void fullCbxPasswords();
    void collectDataAuth();
    void drawRooms(QVariantMap mapRooms);
signals:
    void dataAuthCollected();
};

#endif // MAINWINDOW_H
