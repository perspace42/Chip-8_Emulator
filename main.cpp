#include "mainwindow.h"
#include <QApplication>
#include "Chip8.h"

int main(int argc, char *argv[])
{
    //Test Emulator Functions
    Chip8 myEmulator = Chip8();

    //QTextStream(stdout) << "Done";
    QApplication a(argc, argv);
    MainWindow mainWindow(myEmulator);
    mainWindow.show();
    return a.exec();
}
