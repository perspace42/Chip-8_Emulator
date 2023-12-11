#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QColorDialog>
#include <QColor>
#include <QInputDialog>
#include <QFileDialog>

MainWindow::MainWindow(Chip8& emulator, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), emulatorRef(emulator)
{
    ui->setupUi(this);
    bindKeys = new BindKeys(this);
    scene = new QGraphicsScene(this);
    scene->setBackgroundBrush(Qt::black);
    ui->graphicsView->setScene(scene);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::emulateCycle);
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
        currentColor = color;
    }
}
//This action creates a dialog box that allows the user to enter the processing speed they want the program to run at.
void MainWindow::on_actionSet_Speed_triggered()
{
    bool ok;
    int instructionsPerSecond = QInputDialog::getInt(this, tr("Enter the amount of instructions per second (default: 500)"), tr("Instructions Per Second"), 500, 1, 1001, 1, &ok, Qt::WindowFlags());

    cycleSpeed = (1000 / instructionsPerSecond);
    if(romLoaded){
        timer->start(cycleSpeed);
    }

}
//This action opens a file dialog that requests user to choose a CHIP-8 ROM from the files in their computer
void MainWindow::on_actionLoad_ROM_triggered()
{
    //Open The Dialog
    QString filenamestr = QFileDialog::getOpenFileName(this, "Choose a CHIP-8 ROM");

    //Attempt To Open The File
    try{
        if (filenamestr.isNull() == false){
            QByteArray filenameByteArray = filenamestr.toUtf8();
            const char* filename = filenameByteArray.constData();

            emulatorRef.loadProgram(filename);
            timer->start(cycleSpeed);
            romLoaded = true;

        }
    //If an Exception is thrown (file not valid, file to large) catch the resulting error and close the file
    }catch(std::length_error error){
        errorDialog->showMessage(error.what());
        if (romLoaded){
            on_actionClose_ROM_triggered();
        }
    }catch(std::ios_base::failure error){
        errorDialog->showMessage(error.what());
        if (romLoaded){
            on_actionClose_ROM_triggered();
        }
    }
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
        timer->stop();
    }
    else {
        ui->Pause->setIconText("Pause");
        if(romLoaded){
            timer->start(cycleSpeed);
        }
    }
}

//This shows the bindkeys ui window
void MainWindow::on_actionChange_Keybinds_triggered()
{
    bindKeys->show();
}


void MainWindow::on_actionClose_ROM_triggered()
{
    emulatorRef.clearEmulator();
    ui->graphicsView->scene()->clear();
    timer->stop();
    romLoaded = false;
}

