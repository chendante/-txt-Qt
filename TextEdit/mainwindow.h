#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "searchdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    SearchDialog *sdlg;

public slots:
    void show_findText(QString str);

private slots:
    void on_action_W_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
