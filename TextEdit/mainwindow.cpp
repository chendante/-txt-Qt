#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

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
