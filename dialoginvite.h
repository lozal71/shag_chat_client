#ifndef DIALOGINVITE_H
#define DIALOGINVITE_H

//#if QT_VERSION >= 0x050000
//#include <QtWidgets/QDialog>
//#else
//#include <QtGui/QDialog>
//#endif
#include <QDialog>
#include <QMessageBox>
#include <QDebug>

namespace Ui {
class DialogInvite;
}

class DialogInvite : public QDialog
{
    Q_OBJECT

public:
    explicit DialogInvite(QWidget *parent = nullptr);
    ~DialogInvite();
    QString getUserName();
    QString getTextInvite();
    void fullCbxUserName(QStringList listUsers);
    void fullCbxTextInvite(QStringList listText);
private:
    Ui::DialogInvite *ui;

};

#endif // DIALOGINVITE_H
