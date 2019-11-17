#include "mainwindow.h"
#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    // запуск диалога авторизации
    do{
        // если произошел ввод данных
        if (w.dialogAuth->exec() == QDialog::Accepted){
              // запуск процесса авторизации
            emit w.dataAuthCollected(w.dialogAuth->getLogin(),
                                     w.dialogAuth->getPass());
        }
    } while (w.dialogAuth->authAgain);
    if (!w.dialogAuth->authAgain and !w.dialogAuth->flagExit) {
        w.show();
        a.exec();
    }
    return  0;
}
