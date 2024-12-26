#include "adddialog.h"
#include "ui_adddialog.h"
#include <QMessageBox>
#include <QDebug>

AddDialog::AddDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddDialog)
{
    ui->setupUi(this);
    //去掉问号按钮
    Qt::WindowFlags flags= this->windowFlags();
    setWindowFlags(flags&~Qt::WindowContextHelpButtonHint);

    //设置ID范围
    mIntValidator = new QIntValidator(1, 65535, this);
    ui->lineEditID->setValidator(mIntValidator);
}

AddDialog::~AddDialog()
{
    delete ui;
}

//添加数据
void AddDialog::on_pushButtonAdd_clicked()
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
    emit sig_addData(info);

    //accept();//关闭窗体，并设置返回值为Accepted
}

//取消
void AddDialog::on_pushButtonReturn_clicked()
{
    this->close();
}
