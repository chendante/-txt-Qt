#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    sdlg=new SearchDialog(this);
    QObject::connect(sdlg, SIGNAL(sendString(QString)), this, SLOT(show_findText(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_W_triggered()
{
    sdlg->show();
}

void MainWindow::show_findText(QString str)
{
    if(ui->textEdit->find(str/*,QTextDocument::FindBackward*/))//查找后一个
    {
        // 查找到后高亮显示
        QPalette palette = ui->textEdit->palette();
        palette.setColor(QPalette::Highlight,palette.color(QPalette::Active,QPalette::Highlight));
        ui->textEdit->setPalette(palette);
    }
    else
    {
        QMessageBox::information(this,tr("注意"),tr("没有找到内容"),QMessageBox::Ok);
        ui->textEdit->moveCursor(QTextCursor::Start, QTextCursor::MoveAnchor); //将光标置于开始位置
    }
}

//文件另存为
void MainWindow::on_action_A_triggered()
{
    if(ui->textEdit->toPlainText()==NULL)
    {
        QMessageBox::information(this,tr("注意"),tr("当前文本没有内容"),QMessageBox::Ok);
    }
    else {
        QString fileName = QFileDialog::getSaveFileName(
                        this, tr("save txt file"),
                        "/", tr("Txt files(*.txt)"));
        QFile f1(fileName);
        f1.open(QIODevice::WriteOnly|QIODevice::Text);
        QTextStream str(&f1);
        str<<ui->textEdit->toPlainText();
        f1.close();
    }
}

//打开新文件
void MainWindow::on_action_O_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(
                    this, tr("open txt file"),
                    "/", tr("Txt files(*.txt)"));
    QString displayString;
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"Can't open the file!"<<endl;
    }
    while(!file.atEnd())
    {
        QTextStream txtInput(&file);
        QString str=txtInput.readLine();
        displayString.append(str);
    }
    ui->textEdit->clear();
    ui->textEdit->setPlainText(displayString);
    myfile=fileName;
}

//保存文件
void MainWindow::on_action_S_triggered()
{
    if(myfile==NULL)
    {
        QString fileName = QFileDialog::getSaveFileName(
                        this, tr("save txt file"),
                        "/", tr("Txt files(*.txt)"));
        myfile=fileName;
    }
    QFile f1(myfile);
    f1.open(QIODevice::WriteOnly|QIODevice::Text);
    QTextStream str(&f1);
    str<<ui->textEdit->toPlainText();
    f1.close();
}

//新建文件
void MainWindow::on_action_N_triggered()
{
    if(!(myfile==NULL&&ui->textEdit->toPlainText()==NULL)){
    switch(QMessageBox::question(this,tr("询问"),tr("关闭当前编辑？"),
                QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok))
        {
        case QMessageBox::Ok:
        this->on_action_C_triggered();//关闭当前编辑
            break;
        case QMessageBox::Cancel:
           return;  //放弃新建文件
            break;
        default:
            break;
        }
    }

    QString fileName = QFileDialog::getSaveFileName(
                    this, tr("new txt file"),
                    "/", tr("Txt files(*.txt)"));
    myfile=fileName;
}

//关闭文件
void MainWindow::on_action_C_triggered()
{
    QMessageBox MsgBox;
    MsgBox.setWindowTitle(tr("关闭当前文件编辑"));
    QPushButton *saveButton = MsgBox.addButton(tr("保存"),QMessageBox::ActionRole);
    QPushButton *unsaveButton = MsgBox.addButton(tr("放弃"),QMessageBox::ActionRole);
    QPushButton *cancelButton = MsgBox.addButton(QMessageBox::Cancel);
    if(myfile==NULL)
    {
        if(ui->textEdit->toPlainText()!=NULL)
        {
            MsgBox.setText(tr("当前有未保存内容，是否保存"));
            MsgBox.exec();
        }
        else
        {
            QMessageBox::information(this,tr("提示"),tr("已关闭当前文件编辑"));
        }
    }
    else
    {
        MsgBox.setText(tr("是否保存当前修改"));
        MsgBox.exec();
    }
    if(MsgBox.clickedButton() == saveButton)
    {
       this->on_action_S_triggered();
    }
    if(MsgBox.clickedButton() == unsaveButton)
    {
       QMessageBox::information(this,tr("提示"),tr("已关闭当前文件编辑"));
    }
    if(MsgBox.clickedButton() == cancelButton)
       return;
    myfile.clear();
    ui->textEdit->clear();
}
