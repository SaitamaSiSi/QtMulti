#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QStandardItemModel>
#include "adddialog.h"
#include "modifydialog.h"
#include <QSqlQuery>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    int loadData();

private slots:
    void on_pushButtonAdd_clicked();
    void slot_addData(StudentInfo info);
    void slot_showMenu(QPoint);
    void slot_delData(bool b);
    void on_actionAbout_triggered();
    void slot_modifyData(StudentInfo info);
    void slot_showAddDlg(bool b);
    void slot_modifyDataByDoubleClick(QModelIndex modelIndex);

private:
    Ui::MainWindow *ui;
    QStandardItemModel* mStdItemModel;//表头
    AddDialog* mAddDlg;//添加对话框
    ModifyDialog* mMdfDlg;//修改对话框
    QMenu* mMenu;//右键菜单
    QAction* mDelAction;//菜单删除动作
    QAction* mAddAction;//菜单添加动作
};

#endif // MAINWINDOW_H
