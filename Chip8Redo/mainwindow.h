#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "bindkeys.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_actionColor_triggered();

    void on_actionSet_Speed_triggered();

    void on_actionLoad_ROM_triggered();

    void on_DecreaseSize_clicked();

    void on_IncreaseSize_clicked();

    void on_actionExit_Program_triggered();

    void on_Pause_toggled(bool arg1);

    void on_actionChange_Keybinds_triggered();

private:
    Ui::MainWindow *ui;
    BindKeys *bindKeys;
};
#endif // MAINWINDOW_H

