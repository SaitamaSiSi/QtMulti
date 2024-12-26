#include "logindialog.h"
#include "ui_logindialog.h"
#include <QIntValidator>
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>

QSqlDatabase gDb;//多窗体共用对象

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    //设置窗体标题
    this->setWindowTitle("登录");

    //去掉问号按钮
    Qt::WindowFlags flags= this->windowFlags();
    setWindowFlags(flags&~Qt::WindowContextHelpButtonHint);

    //设置端口范围
    mIntValidator = new QIntValidator(1, 65535, this);
    ui->lineEditPort->setValidator(mIntValidator);

    //填充comboBox 数据库类型
    QStringList drivers = QSqlDatabase::drivers();
    if(drivers.size()>0)
    {
        ui->comboBoxSqlType->addItems(drivers);
    }
    else
    {
        ui->comboBoxSqlType->addItem("None");
        ui->pushButtonInit->setVisible(false);
        ui->pushButtonLogin->setVisible(false);
    }
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

/* 登录
 *
 */
void LoginDialog::on_pushButtonLogin_clicked()
{

    QString sqlType = ui->comboBoxSqlType->currentText();
    if(sqlType.isEmpty() || sqlType == "None")
    {
        QMessageBox::information(this, "提示", "请安装数据库！");
        return;
    }

    QString hostName = ui->lineEditHostName->text();
    QString port = ui->lineEditPort->text();
    QString dataBaseName = ui->lineEditDatabaseName->text();
    QString userName = ui->lineEditUserName->text();
    QString pwd = ui->lineEditPass->text();

    if(hostName.isEmpty() || port.isEmpty() || dataBaseName.isEmpty() || userName.isEmpty())
    {
        return;
    }

    bool ok = connDataBase(mDb, dataBaseName, userName, pwd, hostName, port, sqlType);
    if(ok)
    {
        gDb = mDb;
        accept();//关闭窗体，并设置返回值为Accepted
    }
    else
    {
        QMessageBox::information(this, "提示", "连接失败！");
        //qDebug()<<mDb.lastError().number();
    }

}

/* 连接数据库
 *
 */
bool LoginDialog::connDataBase(QSqlDatabase& db, QString dataBaseName, QString userName, QString Pass,QString hostName, QString port, QString sqlType)
{
    if(db.isDriverAvailable(sqlType))
    {
        if(QSqlDatabase::contains("qt_sql_default_connection"))
        {
            db = QSqlDatabase::database("qt_sql_default_connection");
        }
        else
        {
            db = QSqlDatabase::addDatabase(sqlType);
        }

        db.setHostName(hostName);
        db.setPort(port.toInt());
        db.setDatabaseName(dataBaseName);
        db.setUserName(userName);
        db.setPassword(Pass);

        if(db.open())
        {
            return true;
        }
        db.close();
        db.removeDatabase(sqlType);
    }

    return false;
}

//退出
void LoginDialog::on_pushButtonExit_clicked()
{
    this->close();
}

//用于第一次创建数据库和表
void LoginDialog::on_pushButtonInit_clicked()
{
    QString hostName = ui->lineEditHostName->text();
    QString port = ui->lineEditPort->text();
    QString dataBaseName = ui->lineEditDatabaseName->text();
    QString userName = ui->lineEditUserName->text();
    QString pwd = ui->lineEditPass->text();

    if(hostName.isEmpty() || port.isEmpty() || dataBaseName.isEmpty() || userName.isEmpty())
    {
        return;
    }

    //先使用自带的数据库mysql进行初始化（安装好MySQL数据库就存在的）
    QSqlDatabase myDb;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
    {
        myDb = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        QString sqlType = ui->comboBoxSqlType->currentText();
        if(sqlType.isEmpty() || sqlType == "None")
        {
            QMessageBox::information(this, "提示", "请安装数据库！");
            return;
        }
        if (myDb.isDriverAvailable(sqlType)) {
            myDb = QSqlDatabase::addDatabase(sqlType);
        }
    }

    myDb.setHostName(hostName);
    myDb.setPort(port.toInt());
    myDb.setDatabaseName("mysql");
    myDb.setUserName(userName);
    myDb.setPassword(pwd);
    if(!myDb.open())
    {
        return;
    }

    QSqlQuery sqlQuery(myDb);

    //创建数据库
    sqlQuery.exec(QString("create database if not exists %1").arg(dataBaseName));

    //指定当前使用的数据库
    sqlQuery.exec(QString("use %1").arg(dataBaseName));

    //创建表
    sqlQuery.exec("create table Student(id int(9) not null primary key,name varchar(10) not null);");

    //插入2条数据
    sqlQuery.exec("insert into Student values (1001,'liang'),(1002,'lisi')");

    myDb.close();
    QMessageBox::information(this, "提示", QString("数据库:%1\n 表:Student\n创建完成!").arg(dataBaseName));

}
