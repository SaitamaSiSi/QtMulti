#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDialog>
#include <qfontdialog.h>
#include <QColorDialog>
#include <QTextEdit>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->q_Type, SIGNAL(triggered()), this, SLOT(on_q_Type_clicked()));
}

void MainWindow::on_Text_copyAvailable(bool b)
{   //更新cut，copy，paste的enabled属性
    ui->actCut->setEnabled(b);
    ui->actCopy->setEnabled(b);
    ui->actPaste->setEnabled(ui->Text->canPaste());
}

MainWindow::~MainWindow()
{
    delete ui;
}

//快捷另存为文本操作
void MainWindow::on_q_SaveAs_clicked()
{
    filePath = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("保存"));
        if (filePath.length() == 0) return;
        string text = ui->Text->toPlainText().toStdString();
        string filename = filePath.toStdString();
        FILE *fp = fopen(filename.c_str(), "wb");
        fwrite(text.c_str(), 1, text.length(), fp);
        fclose(fp);
}

//快捷打开文本操作
void MainWindow::on_q_Open_clicked()
{
    filePath = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("打开文件"));
        string filename = filePath.toStdString();
        FILE *fp = fopen(filename.c_str(), "rb");
        char buf[512];
        int n = fread(buf, 1, 512, fp);
        buf[n] = 0;
        fclose(fp);
        QString text = QString::fromStdString(buf);
        ui->Text->setPlainText(text);
}

//快捷新建文本操作
void MainWindow::on_q_Build_clicked()
{
    filePath = QFileDialog::getSaveFileName(this, QString::fromLocal8Bit("新建"));
        if (filePath.length() == 0) return;
    QFile file(filePath);
        if(file.exists())
        {
            qDebug()<<"文件存在";
        }else{

           qDebug()<<"文件不存在,正在新建文件.";
            file.open( QIODevice::ReadWrite | QIODevice::Text );
            file.close();

        }
}

//新建文本
void MainWindow::on_actBuild_triggered()
{
    MainWindow::on_q_Build_clicked();
}

//打开文本
void MainWindow::on_actOpen_triggered()
{
    MainWindow::on_q_Open_clicked();
}

//另存为文本
void MainWindow::on_actSaveAs_triggered()
{
    MainWindow::on_q_SaveAs_clicked();
}

//显示当前时间
void MainWindow::on_actTime_triggered()
{
    time_t t = time( 0 );
    char tmp[64];
    strftime( tmp, sizeof(tmp), "%Y/%m/%d %X %A 本年第%j天",localtime(&t) );
    QString text = QString::fromStdString(tmp);
    ui->Text->setPlainText(text);

}

//快捷字体设置
void MainWindow::on_q_Type_clicked()
{
    /*
    dialog= new Dialog(this);
    dialog->setModal(false);
    dialog->show();
    */
    bool ok;
    QFont font = QFontDialog::getFont(&ok,ui->Text->font(),this,"Font");
    if(ok)
    {
        ui->Text->setFont(font);
    }
}

//字体设置
void MainWindow::on_actType_triggered()
{
    MainWindow::on_q_Type_clicked();
}



void MainWindow::on_actCopy_triggered()
{
    ui->Text->copy();
}

void MainWindow::on_actPaste_triggered()
{
    ui->Text->paste();
}

void MainWindow::on_actCut_triggered()
{
    ui->Text->cut();
}

void MainWindow::on_q_Undo_clicked()
{
    ui->Text->undo();
}

void MainWindow::on_actAllSelect_triggered()
{
    ui->Text->selectAll();
}

void MainWindow::on_q_Redo_clicked()
{
    ui->Text->redo();
}

void MainWindow::on_actRedo_triggered()
{
    MainWindow::on_q_Redo_clicked();
}

void MainWindow::on_actUndo_triggered()
{
    MainWindow::on_q_Undo_clicked();
}

void MainWindow::on_q_Color_clicked()
{
    QColor color = QColorDialog::getColor(Qt::red,this,tr("选择颜色"));
    if(color.isValid())
    {
        ui->Text->setTextColor(color);
    }
}

void MainWindow::on_actColor_triggered()
{
    MainWindow::on_q_Color_clicked();

}

void MainWindow::on_q_Save_clicked()
{
        string text = ui->Text->toPlainText().toStdString();
        string filename = filePath.toStdString();
        FILE *fp = fopen(filename.c_str(), "wb");
        fwrite(text.c_str(), 1, text.length(), fp);
        fclose(fp);
}


void MainWindow::on_actSave_triggered()
{
    MainWindow::on_q_Save_clicked();
}
