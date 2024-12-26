#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QStringList theStrList;
    theStrList << "北京" << "上海" << "天津" << "河北" << "山东" << "四川" << "重庆" ;
    theModel = new QStringListModel(this);
    theModel->setStringList(theStrList);//导入theStrList的内容
    ui->listView->setModel(theModel);//设置数据模型
    ui->listView->setEditTriggers(QAbstractItemView::DoubleClicked |
                                  QAbstractItemView::SelectedClicked);
    //设置为不可编辑
    //ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{//添加一行
    theModel->insertRow(theModel->rowCount());//在尾部插入一空行
    QModelIndex index = theModel->index(theModel->rowCount()-1,0);
    theModel->setData(index, "new item", Qt::DisplayRole);
    ui->listView->setCurrentIndex(index);//设置当前选中的行
}

void MainWindow::on_pushButton_5_clicked()
{//插入一行
    QModelIndex index = ui->listView->currentIndex();
    theModel->insertRow(index.row());
    theModel->setData(index, "inserted item", Qt::DisplayRole);
    ui->listView->setCurrentIndex(index);
}

void MainWindow::on_pushButton_3_clicked()
{//删除当前行
    QModelIndex index = ui->listView->currentIndex();
    theModel->removeRow(index.row());
}

void MainWindow::on_pushButton_4_clicked()
{//清除所有项
    theModel->removeRows(0, theModel->rowCount());
}

void MainWindow::on_pushButton_6_clicked()
{//显示数据模型的StringList
    QStringList tmpList = theModel->stringList();
    ui->plainTextEdit->clear();
    for (int i = 0; i < tmpList.count(); i++)
    {
        ui->plainTextEdit->appendPlainText(tmpList.at(i));
    }
}

void MainWindow::on_listView_clicked(const QModelIndex &index)
{//显示QModelIndex的行丶列号
    ui->LabInfo->setText(QString::asprintf("当前条目: row = %d, column = %d",
            index.row(), index.column()));
}
