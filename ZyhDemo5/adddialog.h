#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>
#include <QIntValidator>

#ifndef STUDENTSTRUCT
//自定义信息结构体
struct StudentInfo
{
    int id;
    QString name;
};
#define STUDENTSTRUCT
#endif

namespace Ui {
class AddDialog;
}

class AddDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddDialog(QWidget *parent = 0);
    ~AddDialog();

signals:
    void sig_addData(StudentInfo info);

private slots:
    void on_pushButtonAdd_clicked();
    void on_pushButtonReturn_clicked();

private:
    Ui::AddDialog *ui;
    QIntValidator* mIntValidator;
};

#endif // ADDDIALOG_H
