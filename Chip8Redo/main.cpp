#include "mainwindow.h"
#include "Chip8.cpp"
#include <QApplication>



int main(int argc, char *argv[])
{
    //Test Emulator Functions
    Chip8 myEmulator = Chip8();

    //QTextStream(stdout) << "Done";
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
