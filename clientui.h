#ifndef CLIENTUI_H
#define CLIENTUI_H
#include <QMainWindow>
//#if QT_VERSION >= 0x050000
//#include <QtWidgets/QMainWindow>
//#else
//#include <QtGui/QMainWindow>
//#endif

namespace Ui {
class clientUI;
}

class clientUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit clientUI(QWidget *parent = nullptr);
    ~clientUI();

private:
    Ui::clientUI *ui;
};

#endif // CLIENTUI_H
