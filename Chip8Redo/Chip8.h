#ifndef CHIP8_H
#define CHIP8_H
//ensure header is only declared once
#include <windows.h> //For Sound Emulation
#include <fstream>   //For File Reading
#include <ctime>     //For Random Number
#include <iostream>  //For Printing Output When Testing and Exception Class
#include <iomanip>   //For Editing Stream Data
#include <string>    //For Exception Messages and Dialog Messages
#include <sstream>   //For Conveting OpCode To Hex Values When Output
#include <QDebug>

class NullOperationException : public std::exception
{
private:
    std::string errorMessage;

public:
    NullOperationException(const std::string &message) : errorMessage(message) {}
    const char *what() const throw()
    {
        return errorMessage.c_str();
    }
};

class UnsupportedLanguageException : public std::exception
{
private:
    std::string errorMessage;

public:
    UnsupportedLanguageException(const std::string &message) : errorMessage(message) {}
    const char *what() const throw()
    {
        return errorMessage.c_str();
    }
};

// Non Class Related Functions
//  Function to set all values in an unsigned char array to a parameter value
void setAllValues(unsigned char *vector, unsigned char value);

// Function to set all values in an unsigned char array, starting from a certain index, to a parameter value
void setAllValues(unsigned char *vector, int startIndex, unsigned char value);

// Function to set all values in an unsigned short array to a parameter value
void setAllValues(unsigned short *vector, unsigned short value);

// Function to set all values in the video int array to a parameter value
void setAllValues(unsigned int (*vector)[64], unsigned int value);

// function to convert the opcode to a hex string for output
std::string toHexString(int number);

class Chip8
{
    // Program Constants

    // Chip8 Memory From 0x000 to 0x1FF is reserved to store the original Chip-8 VM therefore ROM instructions must start at 0x200
    const unsigned int START_ADDRESS = 0x200;
    // Chip8 Memory From 0x050 to 0x0A0 is reserved to store the font
    const unsigned int FONTSET_START_ADDRESS = 0x50;
    // Chip8 Memory Displays Its 16 Characters Using 5 Bytes Each, Therefore This Array Holds 80 Bytes
    const unsigned fontset[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    // Set The Values Within The Function Pointers To Intially Point To Null
    typedef void (Chip8::*Chip8Table)();
    Chip8Table setNull(Chip8Table *table, int length)
    {
        for (int i = 0; i < length; i++)
        {
            table[i] = &Chip8::OP_NULL;
        }
        return *table;
    }

    // Chip-8 Random Number Function
    int getRandom()
    {
        // This seeds the random number generator using the current time
        srand(time(NULL));
        // Generate The Random Number (0 to 255)
        return int(rand() % 256);
    }

    // Public Class Methods
public:
    // Clear The Values Currently In The Emulator
    void clearEmulator()
    {
        setAllValues(registers, static_cast<unsigned char>(0u));
        setAllValues(memory, START_ADDRESS, static_cast<unsigned char>(0u));
        index = 0u;
        pc = START_ADDRESS; // program counter should be at the start address for the next program which is always 0x200
        setAllValues(stack, static_cast<short>(0u));
        sp = 0u;
        delayTimer = 0u;
        soundTimer = 0u;
        setAllValues(keypad, static_cast<short>(0u));
        setAllValues(video, 0u);
        opcode = 0u;
        pcStop = START_ADDRESS; // program stop should also be at the start address until the next program is loaded
    }

    // Load The Program From The File
    void loadProgram(char const *filename)
    {
        // Open the file as a stream of binary and move the file pointer to the end
        std::ifstream file(filename, std::ios::binary | std::ios::ate);

        // If the file is read successfully
        if (file.is_open())
        {
            // First Clear The Variables From The Last Opened File
            clearEmulator();
            // Next Get size of file 
            std::streampos size = file.tellg();
            // If the file is larger than the available memory throw an exception
            if (size > 0x1000){
                throw std::length_error("File size exceeds available memory");
            }

            //allocate a buffer to hold the contents
            char *buffer = new char[size];

            // Go back to the beginning of the file and fill the buffer
            file.seekg(0, std::ios::beg);
            file.read(buffer, size);
            file.close();

            // Load the ROM contents into the emulators memory, starting at 0x200
            for (long i = 0; i < size; ++i)
            {
                memory[START_ADDRESS + i] = buffer[i];
                // Increment The Stop Value
                pcStop += 1;
            }

            // If the stop value is odd (ends in a partial instruction) decrement it by 1
            if (pcStop % 2 != 0){
                --pcStop;
            }

            // Free the buffer
            delete[] buffer;

            // Otherwise Throw An Exception
        }
        else
        {
            throw std::ios_base::failure("ERROR A problem occurred while attempting to open the file");
        }
    }

    // Execute The Next Instruction From The Program
    void nextInstruction()
    {
        //If the program has not reached the end of its instructions
        if (pc < pcStop){

            // First Read The Two-Byte Opcode From Memory
            opcode = (memory[pc] << 8u) | memory[(pc + 1)];
            // Second increment the program counter by 2
            pc += 2;
            // Decode The Opcode Using The Function Table To Determine Which Operation Needs To Be Performed
            ((*this).*(MASTER_TABLE[(opcode & 0xF000u) >> 12u]))();

            // If The Delay Timer Has Been Set, Decrement It
            if (delayTimer > 0)
            {
                --delayTimer;
            }

            // If The Sound Timer Has Been Set, Decrement It And Make Sound
            if (soundTimer > 0)
            {
                // Make Sound
                Beep(300, 10);
                --soundTimer;
            }

        //If the program has reached the end of its instructions (Chip-8 programs do not have a stop character, and therefore should always loop)
        }else{
            //Indicate that the problem is with the program itself
            throw std::out_of_range("ERROR The CHIP-8 program terminated unexpectedly (Out of Instructions)");
        }
    }

    // Public Variables And Constructors
public:
    // This Is The Storage Of The Chip-8 Program, It Contains Sixteen 8 bit registers to Store Program Results
    unsigned char registers[16]{};
    /*
    This Is The Memory Of The Chip-8 Program, It Contains 4096 Bytes Of Memory To Be Used As Follows:
    0x000 - 0x1FF : Originally Used To Store The Chip-8 Interpreter, The Emulator Should Not Use These Values
    0x050 - 0x0A0 : Stores The 16 Built In Characters Of Chip-8 (0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F)
    0x200 - 0xFFF : Program Instructions Are Stored In This Section
    */
    unsigned char memory[0x1000]{};
    // This Is The Index Register of The Chip-8 Program, It contains One 16 bit register To store memory addresses that other operations will make use of
    unsigned short index = 0u;
    // This Is The Program Counter of The Chip-8 Program, It contains One 16 bit register To store the memory address of the next instruction to execute
    unsigned short pc = 0u;
    // This Is The Program Stack, It contains One 16 bit register to store the program order of execution
    unsigned short stack[16]{};
    // This is The Stack Pointer, It contains One 8 bit register to store the memory address to the top of the Stack (The most recently added instruction)
    unsigned char sp = 0u;
    // This is The Built In Delay Timer, It contains One 8 bit register to store a value used for timing in the program
    unsigned char delayTimer = 0u;
    // This is The Built In Sound Timer, It contains One 8 bit register and will play a sound every time it is decremented until reaching 0
    unsigned char soundTimer = 0u;
    /*This is The Key Register, It contains One 8 Bit register to store which input keys are currently being pressed / not being pressed,
    Every key exists in a state of pressed (1) or unpressed (0)*/
    unsigned char keypad[16]{};
    /*This is The Display Memory, It stores which pixels in a 64 x 32 pixel grid have been drawn, each pixel is either on (1) or off (0) pixels
    drawn off screen wrap around to the other side of the screen*/
    unsigned int video[32][64]{};
    // This Is The Operation Code, It stores what instruction is being performed by the emulator.
    unsigned short opcode;

    // This Is The Stop Value, When The Program Counter Reaches This Value The Program Ceases
    unsigned short pcStop = 0x200;

    // Constructor
    Chip8();

    // Private Function Tables and Emulator Functions
private:
    // function pointer table (This Table Redirects To Other Tables And Holds Instructions In Which The Entire OpCode Is Unique)
    Chip8Table MASTER_TABLE[16] = {
        &Chip8::Table0,
        &Chip8::OP_1nnn,
        &Chip8::OP_2nnn,
        &Chip8::OP_3xnn,
        &Chip8::OP_4xnn,
        &Chip8::OP_5xy0,
        &Chip8::OP_6xnn,
        &Chip8::OP_7xnn,
        &Chip8::Table8,
        &Chip8::OP_9xy0,
        &Chip8::OP_Annn,
        &Chip8::OP_Bnnn,
        &Chip8::OP_Cxnn,
        &Chip8::OP_Dxyn,
        &Chip8::TableE,
        &Chip8::TableF};
    // function pointer sub tables Intializes Their Values To Automatically Point To The Command Not Found Function (OP_NULL)
    Chip8Table subTable0[3] = {}; // subtable0 no longer needs any values set to OP_NULL (impossible to call OP_NULL when starting opcode digit is 0)

    Chip8Table subTable8[15] = {setNull(subTable8, 15)};

    Chip8Table subTableE[15] = {setNull(subTableE, 15)};

    Chip8Table subTableF[102] = {setNull(subTableF, 102)};

    // Instruction List Function Implementation

    // Stores A Pointer To The Two Instructions Beginning With: 00
    void Table0()
    {
        // Using The Last 3 Digits
        switch (opcode & 0x0FFF)
        {
            // OP_00E0
        case 0x0E0:
            ((*this).*(subTable0[0]))();
            break;
            // OP_00EE
        case 0x0EE:
            ((*this).*(subTable0[1]))();
            break;
            // OP_0nnn
        default:
            ((*this).*(subTable0[2]))();
        }
        //((*this).*(subTable0[opcode & 0x000Fu]))();
    }

    // Table 0 Functions
    // This operation is invalid, and when run throws an exception explaining why
    void OP_0nnn()
    {
        throw UnsupportedLanguageException("ERROR, The Operation: " + toHexString(opcode) + " Indicates That This Program Is Dependent Upon An Nonexistent Machine Language Subroutine");
    }
    // Clear screen/display
    void OP_00E0()
    {
        setAllValues(video,0u);
    }
    // Return from a subroutine
    void OP_00EE()
    {
        if (sp > 0u)
        { // ensure stack pointer isn't set to zero before excecuting function
            sp--;
            pc = stack[sp]; // decrement stack pointer and set program counter to stack[sp] to undue what was done in function OP_2nnn
        }
        else if(sp == 0)
        {
            throw std::runtime_error("Stack underflow: Stack pointer is 0");
        }
    }

    // Stores A Pointer To The Nine Instructions Beginning With: 8
    void Table8()
    {
        ((*this).*(subTable8[opcode & 0x000Fu]))();
    }

    // Table 8 Functions
    // Store the value of register VY in register VX
    void OP_8xy0()
    {
        unsigned short vxIndex = (opcode & 0x0F00u) >> 8u; // gets x from the four hexadecimal opcode and stores it in vxIndex
        unsigned short vyIndex = (opcode & 0x00F0u) >> 4u; // gets y from the four hexadecimal opcode and stores it in vyIndex

        registers[vxIndex] = registers[vyIndex];
    }
    // Set VX to VX OR VY
    void OP_8xy1()
    {
        unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;
        unsigned short vyIndex = (opcode & 0x00F0u) >> 4u;

        registers[vxIndex] |= registers[vyIndex];
    }
    // Set VX to VX AND VY
    void OP_8xy2()
    {
        unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;
        unsigned short vyIndex = (opcode & 0x00F0u) >> 4u;

        registers[vxIndex] &= registers[vyIndex];
    }
    // Set VX to VX XOR VY
    void OP_8xy3()
    {
        unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;
        unsigned short vyIndex = (opcode & 0x00F0u) >> 4u;

        registers[vxIndex] ^= registers[vyIndex];
    }
    /*Add the value of register VY to register VX
     * Set VF to 01 if a carry occurs
     * Set VF to 00 if a carry does not occur*/
    void OP_8xy4()
    {
        unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;
        unsigned short vyIndex = (opcode & 0x00F0u) >> 4u;
        int sum = registers[vxIndex] + registers[vyIndex];

        registers[vxIndex] += registers[vyIndex];

        if (sum > 0xFFu)
        { // if the sum of VX and VY is greater than 255 (its upper limit), sets VF to 01
            registers[0xF] = 0x01u;
        }
        else
        {
            registers[0xF] = 0x00u;
        }
    }
    /*Subtract the value of register VY from register VX
     * Set VF to 00 if a borrow occurs
     * Set VF to 01 if a borrow does not occur*/
    void OP_8xy5()
    {
        unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;
        unsigned short vyIndex = (opcode & 0x00F0u) >> 4u;
        int difference = registers[vxIndex] - registers[vyIndex];

        registers[vxIndex] -= registers[vyIndex];

        if (difference < 0)
        {
            registers[0xF] = 0x00u;
        }
        else
        {
            registers[0xF] = 0x01u;
        }
    }
    /*Store the value of register VY shifted right one bit in register VX¹
     * Set register VF to the least significant bit prior to the shift
     * VY is unchanged*/
    void OP_8xy6()
    {
        unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;
        unsigned short vyIndex = (opcode & 0x00F0u) >> 4u;

        registers[0xFu] = registers[vyIndex] & 1u;     // get the least significant bit of VY and set VF to it
        registers[vxIndex] = registers[vyIndex] >> 1u; // set VX to VY shifted right one bit
    }
    /*Set register VX to the value of VY minus VX
     * Set VF to 00 if a borrow occurs
     * Set VF to 01 if a borrow does not occur*/
    void OP_8xy7()
    {
        unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;
        unsigned short vyIndex = (opcode & 0x00F0u) >> 4u;
        int difference = registers[vyIndex] - registers[vxIndex];

        registers[vxIndex] = registers[vyIndex] - registers[vxIndex];

        if (difference < 0)
        {
            registers[0xFu] = 0x00u;
        }
        else
        {
            registers[0xFu] = 0x01u;
        }
    }
    /*Store the value of register VY shifted left one bit in register VX¹
     * Set register VF to the most significant bit prior to the shift
     * VY is unchanged*/
    void OP_8xyE()
    {
        unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;
        unsigned short vyIndex = (opcode & 0x00F0u) >> 4u;

        registers[0xFu] = registers[vyIndex] >> 3u;    // get the most significant bit of VY and set VF to it
        registers[vxIndex] = registers[vyIndex] << 1u; // set VX to VY shifted left one bit
    }

    // Stores A Pointer To The Two Instructions Beginning With: E
    void TableE()
    {
        ((*this).*(subTable8[opcode & 0x000Fu]))();
    }

    // Table E Functions
    // Skip the following instruction if the key corresponding to the hex value currently stored in register VX is not pressed
    void OP_ExA1()
    {
        unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;
        unsigned char vxValue = registers[vxIndex];
        qDebug() << "OP_Ex9E()";

        if (keypad[vxValue] == 0)
        {
            pc += 2; // increase program counter by 2 to skip the current instruction
        }
    }
    // Skip the following instruction if the key corresponding to the hex value currently stored in register VX is pressed
    void OP_Ex9E()
    {
        unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;
        unsigned char vxValue = registers[vxIndex];
        qDebug() << "OP_Ex9E()";

        if (keypad[vxValue] != 0)
        {
            pc += 2;
        }
    }

    // Stores A Pointer To The Nine Instructions Beginning With: F
    void TableF()
    {
        ((*this).*(subTableF[opcode & 0x00FFu]))();
    }

    // Table F Functions
    // Store the current value of the delay timer in register VX
    void OP_Fx07()
    {
        unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;

        registers[vxIndex] = delayTimer;
    }
    // Wait for a keypress and store the result in register VX
    void OP_Fx0A()
    {
        unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;
        bool keyPressed = false; // Bool to determine if a key has been pressed


        if (keypad[vxIndex]){
            registers[vxIndex] = static_cast<unsigned char>(vxIndex);
            qDebug() << "register[" <<vxIndex <<"] = " << vxIndex;
            keyPressed = true;
        }else{
            pc -= 2;
        }

    }
    // Set the delay timer to the value of register VX
    void OP_Fx15()
    {
        unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;

        delayTimer = registers[vxIndex];
    }
    // Set the sound timer to the value of register VX
    void OP_Fx18()
    {
        unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;

        soundTimer = registers[vxIndex];
    }
    // Add the value stored in register VX to register I
    void OP_Fx1E()
    {
        unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;

        index += registers[vxIndex];
    }
    // Set I to the memory address of the sprite data corresponding to the hexadecimal digit stored in register VX
    void OP_Fx29()
    {
        unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;
        unsigned char vxValue = registers[vxIndex]; // get the value stored in register VX

        index = FONTSET_START_ADDRESS + (vxValue * 5); // set the index register to the 5-byte sprite in the font set
    }
    // Store the binary-coded decimal equivalent of the value stored in register VX at addresses I (index), I + 1, and I + 2
    void OP_Fx33()
    {
        unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;

        unsigned char hundreds = registers[vxIndex] / 100;    // Get the hundreds digit
        unsigned char tens = (registers[vxIndex] % 100) / 10; // Get the tens digit
        unsigned char units = registers[vxIndex] % 10;        // Get the unit digit

        memory[index] = hundreds; // Places the digits in memory addresses
        memory[index + 1] = tens;
        memory[index + 2] = units;
    }
    /*Store the values of registers V0 to VX inclusive in memory starting at address I
     * I is set to I + X + 1 after operation²*/
    void OP_Fx55()
    {
        unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;

        for (unsigned int i = 0x0u; i <= vxIndex; i++)
        { // loop through and assign memory[index] to a register until VX is reached, then loops 1 more time and exits loop
            memory[index] = registers[i];
            index++; // increment index after each assignment
        }
    }
    /*Fill registers V0 to VX inclusive with the values stored in memory starting at address I
     * I is set to I + X + 1 after operation*/
    void OP_Fx65()
    {
        unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;

        for (unsigned int i = 0x0u; i <= vxIndex; i++)
        { // loop through and assign registers to memory[index] until VX is reached, then loops 1 more time and exits loop
            registers[i] = memory[index];
            index++;
        }
    }

    // Master Table Functions (Functions That Have Unique Identifiers)

    // Jump to address nnn
    void OP_1nnn()
    {
        unsigned short address = (opcode & 0x0FFFu); // get hex memory address from opcode and assign to variable address
        pc = address;                                // set program counter to the obtained address
    }
    // Execute subroutine starting at address NNN
    void OP_2nnn()
    {
        unsigned short address = opcode & 0x0FFFu; // get hexadecimal memory address nnn from the opcode and assign it to a variable
        stack[sp] = pc;                            // program counter is stored in the stack array so the subroutine can be returned from
        ++sp;
        pc = address; // set program counter to the obtained address
    }
    // Skip the following instruction if the value of register VX equals NN
    void OP_3xnn()
    {
        unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;
        unsigned char nn = opcode & 0x00FFu; // gets hexadecimal byte nn from the opcode and assigns it to a variable

        if (registers[vxIndex] == nn)
        {
            pc += 2;
        }
    }
    // Skip the following instruction if the value of register VX is not equal to NN
    void OP_4xnn()
    {
        unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;
        unsigned char nn = opcode & 0x00FFu;

        if (registers[vxIndex] != nn)
        {
            pc += 2;
        }
    }
    // Skip the following instruction if the value of register VX is equal to the value of register VY
    void OP_5xy0()
    {
        unsigned short vxIndex = (opcode & 0x0FFFu) >> 8u;
        unsigned short vyIndex = (opcode & 0x0FFFu) >> 4u;

        if (registers[vxIndex] == registers[vyIndex])
        {
            pc += 2;
        }
    }
    // Store number(nn) in register Vx
    void OP_6xnn()
    {
        unsigned short vxIndex = (opcode & 0x0FFFu) >> 8u;
        unsigned short num = (opcode & 0x00FFu);
        registers[vxIndex] = num;
    }
    // Add number (nn) to register Vx
    void OP_7xnn()
    {
        unsigned short vxIndex = (opcode & 0x0FFFu) >> 8u;
        unsigned short num = (opcode & 0x00FFu);
        registers[vxIndex] += num;
    }
    // Skip the following instruction if the value of register VX is not equal to the value of register VY
    void OP_9xy0()
    {
        unsigned short vxIndex = (opcode & 0x0FFFu) >> 8u;
        unsigned short vyIndex = (opcode & 0x0FFFu) >> 4u;

        if (registers[vxIndex] != registers[vyIndex])
        {
            pc += 2;
        }
    }
    // Store memory address NNN in register I
    void OP_Annn()
    {
        unsigned short address = (opcode & 0x0FFFu);
        index = address;
    }
    // Jump to address NNN + V0
    void OP_Bnnn()
    {
        unsigned short address = opcode & 0x0FFFu;
        address += registers[0];
        pc = address;
    }
    // Set VX to a random number with a mask of NN
    void OP_Cxnn()
    {
        unsigned short vxIndex = (opcode & 0x0FFFu) >> 8u;
        unsigned char nn = opcode & 0x00FFu;

        registers[vxIndex] = (getRandom() & nn);
    }
    // Draw a sprite at position Vx, Vy with n bytes of sprite data starting at the address stored in
    //  'I', set VF to 01 if any set pixels are changed to unset, and 00 otherwise
    void OP_Dxyn()
    {
        unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;
        unsigned short vyIndex = (opcode & 0x00F0u) >> 4u;
        unsigned char height = opcode & 0x000Fu;

        // Get the starting memory address for the sprite data
        unsigned short spriteAddress = index;

        // Reset the collision flag (VF) to 0
        registers[0xF] = 0;

        // Loop through each row of the sprite
        for (unsigned int row = 0; row < height; ++row) {
            // Get the sprite data from memory
            unsigned char spriteData = memory[spriteAddress + row];

            // Loop through each pixel in the row
            for (unsigned int col = 0; col < 8; ++col) {
                // Extract the pixel value from the spriteData
                unsigned char pixelValue = (spriteData & (0x80u >> col)) >> (7 - col);

                // Calculate the position of the pixel on the screen
                int x = (registers[vxIndex] + col) % 64;
                int y = (registers[vyIndex] + row) % 32;

                // XOR the pixel value with the current screen value
                video[y][x] ^= pixelValue;

                // Set VF to 1 if any set pixels are changed to unset
                if (pixelValue == 1 && video[y][x] == 0) {
                    registers[0xF] = 1;
                }
            }
        }
    }

    // Operation Not Found Function
    void OP_NULL()
    {
        throw NullOperationException("ERROR, The Operation: " + toHexString(opcode) + " Is Unrecognized");
    }
};
#endif

