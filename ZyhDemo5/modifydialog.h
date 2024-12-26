#ifndef MODIFYDIALOG_H
#define MODIFYDIALOG_H

#include <QDialog>
#include <QIntValidator>

#ifndef STUDENTSTRUCT
//自定义信息结构体
struct StudentInfo
{
    int id;
    QString name;
};
#endif

namespace Ui {
class ModifyDialog;
}

class ModifyDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ModifyDialog(QWidget *parent = 0);
    ~ModifyDialog();
    void setContent(QString id, QString name);

signals:
    void sig_modifyData(StudentInfo);

private slots:
    void on_pushButtonReturn_clicked();

    void on_pushButtonModify_clicked();

private:
    Ui::ModifyDialog *ui;
    QIntValidator* mIntValidator;
};

#endif // MODIFYDIALOG_H
