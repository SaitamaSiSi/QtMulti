#include "mainwindow.h"
#include "logindialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow *main = new MainWindow();

    LoginDialog login;
    if (login.exec() == QDialog::Accepted)//调用login.exec()，阻塞主控制流，直到完成返回，继续执行主控制流
    {
       main->loadData();
       main->show();
       return a.exec();
    }
    return 0;
}
