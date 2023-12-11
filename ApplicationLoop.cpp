#include "ApplicationLoop.h"


/* //This comment stores the applicationloop member variables as a reminder
class ApplicationLoop {
    // Member Variables
    Chip8 emulator;
    int cycleSpeed;
    bool paused;
    char boundKeys[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    struct CurrentColor {
        unsigned char rVal = 255;
        unsigned char gVal;
        unsigned char bVal;
    };
}
*/
//Constructor
ApplicationLoop::ApplicationLoop(Chip8 &emulatorAddress){
}

//Draw Method Implementation
void ApplicationLoop::draw(int video[32][64]){
}

void ApplicationLoop::setCycleSpeed(int instructionsPerSecond){
}

void setColor(QColor newColor){
    //currentColor = newColor
}
