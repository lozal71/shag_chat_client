#ifndef DIALOGAUTH_H
#define DIALOGAUTH_H

#include <QDialog>
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

private:
    Ui::DialogAuth *ui;
};

#endif // DIALOGAUTH_H
