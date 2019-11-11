#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.show();
    DialogAuth *uiLog = new DialogAuth();
    if(uiLog->exec() == QDialog::Accepted){
        w.getClient()->setLogin(uiLog->getLogin());
        w.getClient()->setPass(uiLog->getPass());
        emit w.dataAuthCollected();
        w.show();
    }
    else {
         QApplication::exit();
    }
    return a.exec();
}
