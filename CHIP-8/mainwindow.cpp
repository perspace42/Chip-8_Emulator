#include "mainwindow.h"
#include "./ui_CHIP-8.h"
#include <QColorDialog>
#include <QColor>
#include <QInputDialog>
#include <QFileDialog>
#include "bindkeys.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    bindKeys = new BindKeys(this);


}

MainWindow::~MainWindow()
{
    delete ui;
}
//This action opens a window with a "color picker" that allows the user to choose a color
void MainWindow::on_actionColor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "Choose Color");
    if(color.isValid()) {

    }
}
//This action creates a dialog box that allows the user to enter the processing speed they want the program to run at.
void MainWindow::on_actionSet_Speed_triggered()
{
    bool ok;
    double speed = QInputDialog::getDouble(this, tr("Enter the speed"), tr("New Speed"), 1, 0, 10, 1, &ok, Qt::WindowFlags(), 0.1);

}
//This action opens a file dialog that requests user to choose a CHIP-8 ROM from the files in their computer
void MainWindow::on_actionLoad_ROM_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this, "Choose a CHIP-8 ROM");
}
//This action decreases the size of the graphics view window
void MainWindow::on_DecreaseSize_clicked()
{
    int x = ui->graphicsView->x();
    int y = ui->graphicsView->y();
    int width = ui->graphicsView->width();
    int height = ui->graphicsView->height();

    if(ui->graphicsView->width() >= 200 && ui->graphicsView->height() >= 200) { //if statement ensures there is a lower limit the size of the window can go to
        ui->graphicsView->setGeometry(x + 15, y + 7, width - 30, height - 14);
    }
}
//This action increases the size of the graphics view window
void MainWindow::on_IncreaseSize_clicked()
{
    int x = ui->graphicsView->x();
    int y = ui->graphicsView->y();
    int width = ui->graphicsView->width();
    int height = ui->graphicsView->height();

    if(ui->graphicsView->width() <= (ui->centralwidget->width() - 80) && ui->graphicsView->height() <= (ui->centralwidget->height() - 40)) { //if statement ensures there is an upper limit the size of the window can go to
        ui->graphicsView->setGeometry(x - 15, y - 7, width + 30, height + 14);
    }
}
//This action exits the emulator
void MainWindow::on_actionExit_Program_triggered()
{
    QCoreApplication::exit(0);
}
//This changes the text of the pause button to Play if its activated, then returns it to Pause when deactivated
void MainWindow::on_Pause_toggled(bool arg1)
{
    if(arg1) {
        ui->Pause->setIconText("Play");
    }
    else {
        ui->Pause->setIconText("Pause");
    }
}

//This shows the bindkeys ui window
void MainWindow::on_actionChange_Keybinds_triggered()
{
    bindKeys->show();
}

