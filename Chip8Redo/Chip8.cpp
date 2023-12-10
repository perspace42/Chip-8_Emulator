/*
Author: Scott Field
Name: Prototype Zero Chip 8 Emulator
Version: 1.0
Date: 12/04/2023
Description:
Form The Framework For The Next Series Of Chip-8 Prototype Emulators To Be Built Off of
*/
#include <windows.h> //For Sound Emulation
#include <fstream>   //For File Reading
#include <ctime>     //For Random Number
#include <iostream>  //For Printing Output When Testing and Exception Class
#include <iomanip>   //For Editing Stream Data
#include <string>    //For Exception Messages and Dialog Messages
#include <sstream>   //For Conveting OpCode To Hex Values When Output
#include "Chip8.h"   //For importing in the rest of the class


// Non Class Related Functions
//  Function to set all values in an unsigned char array to a parameter value
void setAllValues(unsigned char *vector, unsigned char value)
{
    int length = sizeof(vector);
    for (int index = 0; index < length; index++)
    {
        vector[index] = value;
    }
}

// Function to set all values in an unsigned char array, starting from a certain index, to a parameter value
void setAllValues(unsigned char *vector, int startIndex, unsigned char value)
{
    int length = sizeof(vector);
    for (startIndex; startIndex < length; startIndex++)
    {
        vector[startIndex] = value;
    }
}

// Function to set all values in an unsigned short array to a parameter value
void setAllValues(unsigned short *vector, unsigned short value)
{
    int length = sizeof(vector);
    for (int index = 0; index < length; index++)
    {
        vector[index] = value;
    }
}

// Function to set all values in the video int array to a parameter value
void setAllValues(unsigned int (*vector)[64], unsigned int value)
{
    for (int outer = 0; outer < 32; outer++)
    {
        for (int inner = 0; inner < 64; inner++)
        {
            vector[outer][inner] = value;
        }
    }
}

// function to convert the opcode to a hex string for output
std::string toHexString(int number)
{
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(4) << std::hex << number;
    return ss.str();
}

//Redefine Constructor
Chip8::Chip8()
{
    // Initialize Non Fixed Values To 0 On Startup
    setAllValues(registers, static_cast<unsigned char>(0u));
    setAllValues(memory, START_ADDRESS, static_cast<unsigned char>(0u));
    setAllValues(stack, static_cast<short>(0u));
    setAllValues(keypad, static_cast<short>(0u));
    setAllValues(video, 0u);

    // set the first instruction as the next instruction to be executed
    pc = START_ADDRESS;
    for (unsigned int i = 0; i < 80; ++i)
        // loads the font into program memory
        memory[FONTSET_START_ADDRESS + i] = fontset[i];
    // get the first random number
    int randomNumber = getRandom();

    // Overwrite The Command Not Found Function (OP_NULL) In The Sub Tables Where The Other Functions Need To Be Inserted

    // Sub Table 0 Overwrite (This Table Has A Different Structure Than ALL OTHER SUB TABLES)
    subTable0[0] = &Chip8::OP_00E0;
    subTable0[1] = &Chip8::OP_00EE;
    subTable0[2] = &Chip8::OP_0nnn;

    // Sub Table 8 Overwrite
    subTable8[0x0] = &Chip8::OP_8xy0;
    subTable8[0x1] = &Chip8::OP_8xy1;
    subTable8[0x2] = &Chip8::OP_8xy2;
    subTable8[0x3] = &Chip8::OP_8xy3;
    subTable8[0x4] = &Chip8::OP_8xy4;
    subTable8[0x5] = &Chip8::OP_8xy5;
    subTable8[0x6] = &Chip8::OP_8xy6;
    subTable8[0x7] = &Chip8::OP_8xy7;
    subTable8[0xE] = &Chip8::OP_8xyE;

    // Sub Table E Overwrite
    subTableE[0x1] = &Chip8::OP_ExA1;
    subTableE[0xE] = &Chip8::OP_00EE;

    // Sub Table F Overwrite
    subTableF[0x07] = &Chip8::OP_Fx07;
    subTableF[0x0A] = &Chip8::OP_Fx0A;
    subTableF[0x15] = &Chip8::OP_Fx15;
    subTableF[0x18] = &Chip8::OP_Fx18;
    subTableF[0x1E] = &Chip8::OP_Fx1E;
    subTableF[0x29] = &Chip8::OP_Fx29;
    subTableF[0x33] = &Chip8::OP_Fx33;
    subTableF[0x55] = &Chip8::OP_Fx55;
    subTableF[0x65] = &Chip8::OP_Fx65;
}

/*
// Main Is For Testing Any Functions Of The Emulator We Will Comment It Out After Integrating It With The UML
int main()
{
    // Test Emulator Functions
    Chip8 myEmulator = Chip8();

    // Break The Instruction Into Two Halves (Just Like The Memory List Does)
    // Test Instruction 8xy0
    unsigned char firstDigits = 0x81;
    unsigned char secondDigits = 0x10;

    // Assign The First Availble Memory An Instruction
    myEmulator.memory[0x200] = firstDigits;
    myEmulator.memory[0x201] = secondDigits;
    myEmulator.memory[0x202] = 0xD1;
    myEmulator.memory[0x203] = 0x23;
    myEmulator.memory[0x204] = 0x11;

    // Increment The Stop Value (This Is Done Automatically When Loading A File) It is being set here for testing
    myEmulator.pcStop += 4; 

    boolean finished = false;
    while (! finished)
    {
        // Try To Execute The Instruction
        try
        {
            myEmulator.nextInstruction();
        }
        catch (const std::exception &e)
        {
            std::cout << "Caught exception: " << e.what() << "\n";
            finished = true;
            break;
        }
        std::cout << "Instruction: " + toHexString(myEmulator.opcode) + " executed\n";
    }
    std::cout << "Done";
}
*/
