#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

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
    myfile.clear();
    ui->textEdit->clear();
    QString fileName = QFileDialog::getSaveFileName(
                    this, tr("new txt file"),
                    "/", tr("Txt files(*.txt)"));
    myfile=fileName;
}

//关闭文件
void MainWindow::on_action_C_triggered()
{
    myfile.clear();
    ui->textEdit->clear();
}
