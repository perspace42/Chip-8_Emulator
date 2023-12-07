#include "mainwindow.h"

#include <QApplication>

/*
Author: Scott Field
Name: Application Loop
Date: 12/04/2023
Version: 0.1
Description:
Handle manipulation of the output of the Chip8 emulator
*/
#include "Chip8.cpp"
class Chip8;
class ApplicationLoop{
    //Member Variables
    Chip8 emulator;
    int cycleSpeed;
    boolean paused;
    char boundKeys [16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    struct CurrentColor{
        unsigned char rVal = 255;
        unsigned char gVal;
        unsigned char bVal;
    };

public:
    //Constructor
    ApplicationLoop(Chip8 &emulatorAddress) : emulator(emulatorAddress){}
    //Methods
    void draw(int video[32][64]){}
    void setCycleSpeed(int instructionsPerSecond){}
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
