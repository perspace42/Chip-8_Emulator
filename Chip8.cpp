/*
Author: Scott Field
Name: Prototype Zero Chip 8 Emulator
Version: 1.0
Date: 11/09/2023
Description:
To Test Out The Prototype Zero Chip 8 Emulator
*/
#include <windows.h> //For Sound Emulation

class Chip8
{
    //Chip8 Memory From 0x000 to 0x1FF is reserved therefore ROM instructions must start at 0x200
    const unsigned int START_ADDRESS = 0x200;

public:
    //This Is The Storage Of The Chip-8 Program, It Contains Sixteen 8 bit registers to Store Program Results
	unsigned char registers[16]{};
    /*
    This Is The Memory Of The Chip-8 Program, It Contains 4096 Bytes Of Memory To Be Used As Follows:
    0x000 - 0x1FF : Originally Used To Store The Chip-8 Interpreter, The Emulator Should Not Use These Values
    0x050 - 0x0A0 : Stores The 16 Built In Characters Of Chip-8 (0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F)
    0x200 - 0xFFF : Program Instructions Are Stored In This Section
    */
	unsigned char memory[0x1000]{};
    //This Is The Index Register of The Chip-8 Program, It contains One 16 bit register To store memory addresses that other operations will make use of
	unsigned short index{};
    //This Is The Program Counter of The Chip-8 Program, It contains One 16 bit register To store the memory address of the next instruction to execute
	unsigned short pc{};
    //This Is The Program Stack, It contains One 16 bit register to store the program order of execution (This may later use a C++ Stack Instead of a C++ Vector)
	unsigned short stack[16]{}; 
    //This is The Stack Pointer, It contains One 8 bit register to store the memory address to the top of the Stack (The most recently added instruction)
	unsigned char sp{};
    //This is The Built In Delay Timer, It contains One 8 bit register to store a value used for timing in the program (We will set it to delay based on clock speed)
	unsigned char delayTimer{};
    //This is The Built In Sound Timer, It contains One 8 bit register and will play a sound every time it is decremented until reaching 0 
	unsigned char soundTimer{};
    /*This is The Key Register, It contains One 8 Bit register to store which input keys execute which instructions, 
    Every key exists in a state of pressed (1) or unpressed (0)*/
	unsigned char keypad[16]{};
    /*This is The Display Memory, It stores which pixels in a 64 x 32 pixel grid have been drawn, each pixel is either on (1) or off (0) pixels
    drawn off screen wrap around to the other side of the screen*/
	unsigned int video[64 * 32]{};
    //This Is The Operation Code, It stores what instruction is being performed by the emulator.
	unsigned short opcode;

    //Constructor
    Chip8()
    {
        pc = START_ADDRESS; //set the first instruction as the next instruction to be executed
    }
};

int main(){
    //testing the sound feature (The first argument is the Hertz (Beep Frequency) and the second argument is the duration in milliseconds)
    Beep(440,500);
}
