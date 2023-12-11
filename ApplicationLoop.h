#ifndef APPLICATIONLOOP_H
#define APPLICATIONLOOP_H
#include "Chip8.h"
#include <QColor>

class ApplicationLoop {
    // Member Variables
    Chip8 emulator;
    int cycleSpeed;
    bool paused;
    char boundKeys[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    QColor currentColor = Qt::white;

public:
    // Constructor
    ApplicationLoop(Chip8 &emulatorAddress);

    // Methods
    void draw(int video[32][64]);
    void setCycleSpeed(int instructionsPerSecond);
    void setColor(QColor newColor);
};

#endif

