#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //LoginDialog * ldlg = new LoginDialog();
    //DialogAuth *ldlg = new DialogAuth();
//    if(ldlg->exec() == QDialog::Accepted){
//        w.show();
//    }
    return a.exec();
}
