#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QIntValidator>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();
    bool connDataBase(QSqlDatabase& db, QString dataBaseName, QString userName, QString Pass, QString hostName = "127.0.0.1", QString port = "3306", QString sqlType = "QMYSQL");
    void setSqlDatabase(QSqlDatabase& db);

private slots:
    void on_pushButtonLogin_clicked();

    void on_pushButtonExit_clicked();

    void on_pushButtonInit_clicked();

private:
    Ui::LoginDialog *ui;
    QSqlDatabase mDb;
    QIntValidator* mIntValidator;
};

#endif // LOGINDIALOG_H
