#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <string>
#include <QFile>
#include <QDebug>


using namespace std;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_q_SaveAs_clicked();

    void on_q_Open_clicked();

    void on_q_Build_clicked();

    void on_actBuild_triggered();

    void on_actOpen_triggered();

    void on_actSaveAs_triggered();

    void on_actTime_triggered();

    void on_q_Type_clicked();

    void on_actType_triggered();

    void on_q_Undo_clicked();

    void on_Text_copyAvailable(bool b);

    void on_actCopy_triggered();

    void on_actPaste_triggered();

    void on_actCut_triggered();

    void on_actAllSelect_triggered();

    void on_q_Redo_clicked();

    void on_actRedo_triggered();

    void on_actUndo_triggered();

    void on_q_Color_clicked();

    void on_actColor_triggered();

    void on_q_Save_clicked();

    void on_actSave_triggered();

private:
    Ui::MainWindow *ui;
    QString filePath;
};

#endif // MAINWINDOW_H
