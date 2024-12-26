#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>

//要操作的数据库表
#define TABLENAME "Student"

extern QSqlDatabase gDb;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //注册自定义数据类型（作为信号参数）
    qRegisterMetaType<StudentInfo>("StudentInfo");

    //设置窗体标题
    this->setWindowTitle("主界面");

    //屏蔽最大化按钮
    Qt::WindowFlags flags= this->windowFlags();
    setWindowFlags(flags&~Qt::WindowMaximizeButtonHint);

    //固定窗体大小
    this->setFixedSize(this->width(), this->height()-23);

    //创建表头
    mStdItemModel = new QStandardItemModel();
    //添加两列
    mStdItemModel->setColumnCount(2);
    mStdItemModel->setHeaderData(0,Qt::Horizontal,"ID");
    mStdItemModel->setHeaderData(1,Qt::Horizontal,"姓名");

    //设置到tableView控件
    ui->tableViewDataList->setModel(mStdItemModel);

    //设置tableView可以整行选中
    ui->tableViewDataList->setSelectionBehavior(QAbstractItemView::SelectRows);

    //设置表头背景色
    //ui->tableViewDataList->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}");

    //设置不可双击编辑
    ui->tableViewDataList->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //添加窗体，修改窗体信号与槽连接需要非空对象
    mAddDlg = new AddDialog();
    mMdfDlg = new ModifyDialog();

    //初始化右键菜单对象
    mMenu = new QMenu(ui->tableViewDataList);
    mDelAction = mMenu->addAction("删除");
    mAddAction = mMenu->addAction("添加");

    //连接信号槽函数，接收并处理 添加页面发来的数据
    connect(mAddDlg, SIGNAL(sig_addData(StudentInfo)), this, SLOT(slot_addData(StudentInfo)));

    //设置菜单相应策略
    ui->tableViewDataList->setContextMenuPolicy(Qt::CustomContextMenu);

    //连接右键菜单 信号与槽
    connect(ui->tableViewDataList, SIGNAL(customContextMenuRequested(QPoint)),this, SLOT(slot_showMenu(QPoint)));
    connect(mDelAction, SIGNAL(triggered(bool)), this, SLOT(slot_delData(bool)));
    connect(mAddAction, SIGNAL(triggered(bool)), this, SLOT(slot_showAddDlg(bool)));

    //双击列表项时处理
    connect(ui->tableViewDataList, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slot_modifyDataByDoubleClick(QModelIndex)));

    //修改窗体回传修改后的数据到主界面
    connect(mMdfDlg, SIGNAL(sig_modifyData(StudentInfo)), this, SLOT(slot_modifyData(StudentInfo)));

}

MainWindow::~MainWindow()
{
    delete ui;
}

//从数据库加载数据
int MainWindow::loadData()
{
    QSqlQuery sqlQuery(gDb);
    QString msg = "主机名:" + gDb.hostName() \
                + "\n数据库:" +gDb.databaseName() \
                + "\n用户名:" + gDb.userName();
    ui->labelMsg->setText(msg);
    ui->labelMsg->setToolTip(msg);

    bool ok = gDb.open();
    if(ok)
    {
        QString sql = "select * from " + QString(TABLENAME);
        ok = sqlQuery.exec(sql);
    }

    if(!ok)
    {
        //添加测试数据
        mStdItemModel->setItem(0,0,new QStandardItem("1001"));
        mStdItemModel->setItem(0,1,new QStandardItem("Liang"));
        return 0;
    }

    int i = 0;
    while(sqlQuery.next())
    {
        //qDebug()<<sqlQuery.value("id").toString()<<","<<sqlQuery.value("name").toString();
        mStdItemModel->setItem(i,0,new QStandardItem(sqlQuery.value("id").toString()));
        mStdItemModel->setItem(i,1,new QStandardItem(sqlQuery.value("name").toString()));
        i++;
    }

    QString tips;
    tips = tips.asprintf("成功加载 %d 条数据!", i);
    ui->statusBar->setToolTip(tips);
    ui->statusBar->showMessage(tips);
    return 0;
}

//添加数据
void MainWindow::on_pushButtonAdd_clicked()
{
    if(nullptr == mAddDlg)
    {
        mAddDlg = new AddDialog();
    }
    mAddDlg->exec();
}

//处理 来自添加页面的数据
void MainWindow::slot_addData(StudentInfo info)
{    
    QSqlQuery sqlQuery(gDb);

    //插入语句："insert into Student values (1001,'liang')"
    QString sql = "insert into "+QString(TABLENAME)+" values (" +QString::number(info.id) +",'" + info.name +"')";

    QString result = "添加成功!";
    if(sqlQuery.exec(sql))
    {
        int count = mStdItemModel->rowCount();
        mStdItemModel->setItem(count,0,new QStandardItem(QString::number(info.id)));
        mStdItemModel->setItem(count,1,new QStandardItem(info.name));
        result += "id:" + QString::number(info.id);
    }
    else
    {
        result = "添加失败："+sqlQuery.lastError().text();
    }
    ui->statusBar->setToolTip(result);
    ui->statusBar->showMessage(result);
}

//显示右键菜单
void MainWindow::slot_showMenu(QPoint)
{
    mMenu->exec(QCursor::pos());//在当前鼠标位置显示
}

//删除选中项
void MainWindow::slot_delData(bool b)
{
    Q_UNUSED(b);

    QSqlQuery sqlQuery(gDb);

    //获取的是列索引 ui->tableViewDataList->selectionModel()->currentIndex();
    QModelIndexList indexList = ui->tableViewDataList->selectionModel()->selectedRows();
    QModelIndex index;

    QString id;
    int i, sel, sucess = 0;
    int size = indexList.size();
    for(i=size-1; i>=0; i--)//倒着删除，顺着删除要处理索引偏移
    {
        index = indexList.at(i);
        sel = index.row();
        id= mStdItemModel->data(mStdItemModel->index(index.row(),0)).toString();//第row行第0列的内容
        //根据ID删除语句 "delete from Student where id='1001'"
        QString sql = "delete from "+QString(TABLENAME)+" where id='"+id+"'";
        if(sqlQuery.exec(sql))
        {
            sucess++;
            mStdItemModel->removeRow(sel);
        }
    }

    if(size>0)
    {
        QString tips;
        tips = tips.asprintf("选中 %d条数据，成功删除 %d 条数据！", size, sucess);
        ui->statusBar->setToolTip(tips);
        ui->statusBar->showMessage(tips);
    }

}

//添加
void MainWindow::slot_showAddDlg(bool b)
{
    Q_UNUSED(b);
    mAddDlg->exec();
}

//菜单的关于动作
void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this, "关于", "这是操作数据库的简单例子。");
}

//弹出修改数据窗口
void MainWindow::slot_modifyDataByDoubleClick(QModelIndex modelIndex)
{
    Q_UNUSED(modelIndex);

    QString id,name;
    int row;
    QModelIndex index = ui->tableViewDataList->currentIndex();
    if(index.isValid())
    {
        row = index.row();
        id= mStdItemModel->data(mStdItemModel->index(row,0)).toString();//第row行第0列的内容
        name= mStdItemModel->data(mStdItemModel->index(row,1)).toString();//第row行第1列的内容
        mMdfDlg->setContent(id, name);
        mMdfDlg->exec();
    }
}

//修改数据
void MainWindow::slot_modifyData(StudentInfo info)
{
    QSqlQuery sqlQuery(gDb);

    //修改语句：update Student set name='liang' where id=1001;
    QString sql = "update "+QString(TABLENAME)+" set name='" + info.name +"' where id=" + QString::number(info.id);

    QString result = "修改成功!";
    if(sqlQuery.exec(sql))
    {
        int i;
        QString id;
        int count = mStdItemModel->rowCount();
        for(i=0; i<count; i++)//根据id找到要修改的数据项
        {
            id = mStdItemModel->data(mStdItemModel->index(i,0)).toString();
            if(info.id == id.toInt())
            {
                mStdItemModel->setItem(i,0,new QStandardItem(QString::number(info.id)));
                mStdItemModel->setItem(i,1,new QStandardItem(info.name));
                break;
            }
        }
    }
    else
    {
        result = "修改失败:"+sqlQuery.lastError().text();
    }
    ui->statusBar->setToolTip(result);
    ui->statusBar->showMessage(result);
}
