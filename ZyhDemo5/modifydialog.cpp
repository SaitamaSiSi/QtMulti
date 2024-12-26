#include "modifydialog.h"
#include "ui_modifydialog.h"
#include <QMessageBox>

ModifyDialog::ModifyDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ModifyDialog)
{
    ui->setupUi(this);
    //去掉问号按钮
    Qt::WindowFlags flags= this->windowFlags();
    setWindowFlags(flags&~Qt::WindowContextHelpButtonHint);

    //设置ID范围
    mIntValidator = new QIntValidator(1, 65535, this);
    ui->lineEditID->setValidator(mIntValidator);
}

void ModifyDialog::setContent(QString id, QString name)
{
    ui->lineEditID->setText(id);
    ui->lineEditName->setText(name);
}

ModifyDialog::~ModifyDialog()
{
    delete ui;
}

//返回
void ModifyDialog::on_pushButtonReturn_clicked()
{
    this->close();
}

//修改
void ModifyDialog::on_pushButtonModify_clicked()
{
    StudentInfo info;
    QString sid = ui->lineEditID->text();
    QString name = ui->lineEditName->text();
    if(sid.isEmpty() || name.isEmpty())
    {
        QMessageBox::information(this, "提示", "ID和姓名不能为空");
        return;
    }

    info.id = sid.toInt();
    info.name = name;

    //将数据发送主界面
    emit sig_modifyData(info);
}
