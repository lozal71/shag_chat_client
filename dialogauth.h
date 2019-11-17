#ifndef DIALOGAUTH_H
#define DIALOGAUTH_H

#include <QDialog>
#include <QMessageBox>
#include <QDebug>
//#if QT_VERSION >= 0x050000
//#include <QtWidgets/QDialog>
//#else
//#include <QtGui/QDialog>
//#endif

namespace Ui {
class DialogAuth;
}

class DialogAuth : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAuth(QWidget *parent = nullptr);
    ~DialogAuth();
    QString getLogin();
    QString getPass();
    bool authAgain;
    bool flagExit;
    void showWarningAuth(QString sParam);
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::DialogAuth *ui;
    QString login;
    QString pass;
    void fullCbxLogins();
    void fullCbxPasswords();

};

#endif // DIALOGAUTH_H
