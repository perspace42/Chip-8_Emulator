/*
Author: Scott Field
Name: Prototype Zero Chip 8 Emulator
Version: 1.0
Date: 12/02/2023
Description:
Form The Framework For The Next Series Of Chip-8 Prototype Emulators To Be Built Off of
*/
#include <windows.h> //For Sound Emulation
#include <fstream> //For File Reading
#include <ctime> //For Random Number
#include <iostream> //For Printing Output When Testing and Exception Class
#include <iomanip> //For Editing Stream Data
#include <string> //For Exception Messages and Dialog Messages
#include <sstream> //For Conveting OpCode To Hex Values When Output

//Custom Exceptions
class NullOperationException : public std::exception{
private:
   std::string errorMessage;
public:
    NullOperationException(const std::string& message) : errorMessage(message) {} 
    const char* what() const throw() {
        return errorMessage.c_str();
    }
};

class UnsupportedLanguageException : public std::exception{
private:
   std::string errorMessage;
public:
    UnsupportedLanguageException(const std::string& message) : errorMessage(message) {} 
    const char* what() const throw() {
        return errorMessage.c_str();
    }
};

//Non Class Related Functions
// Function to set all values in an unsigned char array to a parameter value
void setAllValues(unsigned char* vector, unsigned char value) {
    int length = sizeof(vector);
    for (int index = 0; index < length; index++) {
        vector[index] = value;
    }
}

// Function to set all values in an unsigned short array to a parameter value
void setAllValues(unsigned short* vector, unsigned short value) {
    int length = sizeof(vector);
    for (int index = 0; index < length; index++) {
        vector[index] = value;
    }
}

// Function to set all values in the video int array to a parameter value
void setAllValues(unsigned int(*vector)[64], unsigned int value) {
    for (int outer = 0; outer < 32; outer++) {
        for (int inner = 0; inner < 64; inner++) {
            vector[outer][inner] = value;
        }
    }
}

//function to convert the opcode to a hex string for output
std::string toHexString(int number) {
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(4) << std::hex << number;
    return ss.str();
}

//Emulator Class Start
class Chip8
{
    //Program Constants

    //Chip8 Memory From 0x000 to 0x1FF is reserved to store the original Chip-8 VM therefore ROM instructions must start at 0x200
    const unsigned int START_ADDRESS = 0x200;
    //Chip8 Memory From 0x050 to 0x0A0 is reserved to store the font
    const unsigned int FONTSET_START_ADDRESS = 0x50;
    //Chip8 Memory Displays Its 16 Characters Using 5 Bytes Each, Therefore This Array Holds 80 Bytes
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

    //Set The Values Within The Function Pointers To Intially Point To Null
    typedef void (Chip8::* Chip8Table)();
    Chip8Table setNull(Chip8Table* table, int length) {
        for (int i = 0; i < length; i++) {
            table[i] = &OP_NULL;
        }
        return *table;
    }

    //Chip-8 Random Number Function
    int getRandom() {
        //This seeds the random number generator using the current time
        srand(time(NULL));
        //Generate The Random Number (0 to 255)
        return int(rand() % 256);
    }

    //Public Class Methods
public:
    //Clear The Values Currently In The Emulator
    void clearEmulator() {
        setAllValues(registers, static_cast<unsigned char>(0u));
        setAllValues(memory, static_cast<unsigned char>(0u));
        index = 0u;
        pc = START_ADDRESS; //program counter should be at the start address for the next program which is always 0x200
        setAllValues(stack, static_cast<short>        (0u));
        sp = 0u;
        delayTimer = 0u;
        soundTimer = 0u;
        setAllValues(keypad, static_cast<short>        (0u));
        setAllValues(video, 0u);
        opcode = 0u;
    }

    //Load The Program From The File
    void loadProgram(char const* filename) {
        // Open the file as a stream of binary and move the file pointer to the end
        std::ifstream file(filename, std::ios::binary | std::ios::ate);

        //If the file is read successfully
        if (file.is_open())
        {
            // First Clear The Variables From The Last Opened File
            clearEmulator();
            // Next Get size of file and allocate a buffer to hold the contents
            std::streampos size = file.tellg();
            char* buffer = new char[size];

            // Go back to the beginning of the file and fill the buffer
            file.seekg(0, std::ios::beg);
            file.read(buffer, size);
            file.close();

            // Load the ROM contents into the emulators memory, starting at 0x200
            for (long i = 0; i < size; ++i)
            {
                memory[START_ADDRESS + i] = buffer[i];
            }

            // Free the buffer
            delete[] buffer;
        
        //Otherwise Throw An Exception
        }else{
            throw std::ios_base::failure("ERROR A problem occurred while attempting to open the file");
        }
    }

    //Execute The Next Instruction From The Program
    void nextInstruction() {
        //First Read The Two-Byte Opcode From Memory 
        opcode = (memory[pc] << 8u) | memory[(pc + 1)];
        //Second increment the program counter by 2
        pc += 2;
        //Decode The Opcode Using The Function Table To Determine Which Operation Needs To Be Performed
        ((*this).*(MASTER_TABLE[(opcode & 0xF000u) >> 12u]))();

        //If The Delay Timer Has Been Set, Decrement It
        if (delayTimer > 0) {
            --delayTimer;
        }

        //If The Sound Timer Has Been Set, Decrement It
        if (soundTimer > 0) {
            --soundTimer;
        }

    }


    //Public Variables And Constructors
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
    unsigned short index = 0u;
    //This Is The Program Counter of The Chip-8 Program, It contains One 16 bit register To store the memory address of the next instruction to execute
    unsigned short pc = 0u;
    //This Is The Program Stack, It contains One 16 bit register to store the program order of execution 
    unsigned short stack[16]{};
    //This is The Stack Pointer, It contains One 8 bit register to store the memory address to the top of the Stack (The most recently added instruction)
    unsigned char sp = 0u;
    //This is The Built In Delay Timer, It contains One 8 bit register to store a value used for timing in the program 
    unsigned char delayTimer = 0u;
    //This is The Built In Sound Timer, It contains One 8 bit register and will play a sound every time it is decremented until reaching 0 
    unsigned char soundTimer = 0u;
    /*This is The Key Register, It contains One 8 Bit register to store which input keys are currently being pressed / not being pressed,
    Every key exists in a state of pressed (1) or unpressed (0)*/
    unsigned char keypad[16]{};
    /*This is The Display Memory, It stores which pixels in a 64 x 32 pixel grid have been drawn, each pixel is either on (1) or off (0) pixels
    drawn off screen wrap around to the other side of the screen*/
    unsigned int video[32][64]{};
    //This Is The Operation Code, It stores what instruction is being performed by the emulator.
    unsigned short opcode;

    //Constructor
    Chip8()
    {
        //Initialize Vector Values To 0 On Startup
        setAllValues(registers, static_cast<unsigned char>(0u));
        setAllValues(memory, static_cast<unsigned char>(0u));
        setAllValues(stack, static_cast<short>        (0u));
        setAllValues(keypad, static_cast<short>        (0u));
        setAllValues(video, 0u);

        //set the first instruction as the next instruction to be executed
        pc = START_ADDRESS;
        for (unsigned int i = 0; i < 80; ++i)
            //loads the font into program memory
            memory[FONTSET_START_ADDRESS + i] = fontset[i];
        //get the first random number
        int randomNumber = getRandom();

        //Overwrite The Command Not Found Function (OP_NULL) In The Sub Tables Where The Other Functions Need To Be Inserted

        //Sub Table 0 Overwrite (This Table Has A Different Structure Than ALL OTHER SUB TABLES)
        subTable0[0] = &OP_00E0;
        subTable0[1] = &OP_00EE;
        subTable0[2] = &OP_0nnn;

        //Sub Table 8 Overwrite
        subTable8[0x0] = &OP_8xy0;
        subTable8[0x1] = &OP_8xy1;
        subTable8[0x2] = &OP_8xy2;
        subTable8[0x3] = &OP_8xy3;
        subTable8[0x4] = &OP_8xy4;
        subTable8[0x5] = &OP_8xy5;
        subTable8[0x6] = &OP_8xy6;
        subTable8[0x7] = &OP_8xy7;
        subTable8[0xE] = &OP_8xyE;

        //Sub Table E Overwrite
        subTableE[0x1] = &OP_ExA1;
        subTableE[0xE] = &OP_00EE;

        //Sub Table F Overwrite
        subTableF[0x07] = &OP_Fx07;
        subTableF[0x0A] = &OP_Fx0A;
        subTableF[0x15] = &OP_Fx15;
        subTableF[0x18] = &OP_Fx18;
        subTableF[0x1E] = &OP_Fx1E;
        subTableF[0x29] = &OP_Fx29;
        subTableF[0x33] = &OP_Fx33;
        subTableF[0x55] = &OP_Fx55;
        subTableF[0x65] = &OP_Fx65;
    }

    //Private Function Tables and Emulator Functions
private:
    //function pointer table (This Table Redirects To Other Tables And Holds Instructions In Which The Entire OpCode Is Unique)
    Chip8Table MASTER_TABLE[16] = {
        &Table0,
        &OP_1nnn,
        &OP_2nnn,
        &OP_3xnn,
        &OP_4xnn,
        &OP_5xy0,
        &OP_6xnn,
        &OP_7xnn,
        &Table8,
        &OP_9xy0,
        &OP_Annn,
        &OP_Bnnn,
        &OP_Cxnn,
        &OP_Dxyn,
        &TableE,
        &TableF
    };
    //function pointer sub tables Intializes Their Values To Automatically Point To The Command Not Found Function (OP_NULL)
    Chip8Table subTable0[3] = {}; //subtable0 no longer needs any values set to OP_NULL (impossible to call OP_NULL when starting opcode digit is 0)

    Chip8Table subTable8[15] = { setNull(subTable8,15) };

    Chip8Table subTableE[15] = { setNull(subTableE,15) };

    Chip8Table subTableF[102] = { setNull(subTableF,102) };

    //Instruction List Function Implementation

    //Stores A Pointer To The Two Instructions Beginning With: 00
    void Table0() {
        //Using The Last 3 Digits
        switch (opcode & 0x0FFF) {
            //OP_00E0
        case 0x0E0:
            ((*this).*(subTable0[0]))();
            //OP_00EE
        case 0x0EE:
            ((*this).*(subTable0[1]))();
            //OP_0nnn
        default:
            ((*this).*(subTable0[2]))();
        }
        //((*this).*(subTable0[opcode & 0x000Fu]))();
    }

        //Table 0 Functions
            //This operation is invalid, and when run throws an exception explaining why
            void OP_0nnn() {
                throw UnsupportedLanguageException("ERROR, The Operation: " + toHexString(opcode) + " Indicates That This Program Is Dependent Upon An Nonexistent Machine Language Subroutine");
            }

            void OP_00E0(){
            }
            //Return from a subroutine
            void OP_00EE(){  
                if (sp > 0u) {//ensure stack pointer isn't set to zero before excecuting function
                    sp--;
                    pc = stack[sp];//decrement stack pointer and set program counter to stack[sp] to undue what was done in function OP_2nnn
                }
                else { //Might want to add exception handling here in case this function is called and sp = 0
                }
            }

        //Stores A Pointer To The Nine Instructions Beginning With: 8
        void Table8(){
            ((*this).*(subTable8[opcode & 0x000Fu]))();
        }

        //Table 8 Functions
            //Store the value of register VY in register VX
            void OP_8xy0(){
                unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;//gets x from the four hexadecimal opcode and stores it in vxIndex
                unsigned short vyIndex = (opcode & 0x00F0u) >> 4u;//gets y from the four hexadecimal opcode and stores it in vyIndex

                registers[vxIndex] = registers[vyIndex];
            }
            //Set VX to VX OR VY
            void OP_8xy1(){
                unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;
                unsigned short vyIndex = (opcode & 0x00F0u) >> 4u;

                registers[vxIndex] |= registers[vyIndex];
            }
            //Set VX to VX AND VY
            void OP_8xy2(){
                unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;
                unsigned short vyIndex = (opcode & 0x00F0u) >> 4u;

                registers[vxIndex] &= registers[vyIndex];
            }
            //Set VX to VX XOR VY
            void OP_8xy3(){
                unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;
                unsigned short vyIndex = (opcode & 0x00F0u) >> 4u;

                registers[vxIndex] ^= registers[vyIndex];
            }
            /*Add the value of register VY to register VX
            * Set VF to 01 if a carry occurs
            * Set VF to 00 if a carry does not occur*/
            void OP_8xy4(){
                unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;
                unsigned short vyIndex = (opcode & 0x00F0u) >> 4u;

                registers[vxIndex] += registers[vyIndex];

                if (registers[vxIndex] > 0xFFu) {//if VX is greater than 255 (its upper limit) after adding VY to it, sets VF to 01
                    registers[0xF] = 0x01u;
                }
                else {
                    registers[0xF] = 0x00u;
                }
            }
            /*Subtract the value of register VY from register VX
             * Set VF to 00 if a borrow occurs
             * Set VF to 01 if a borrow does not occur*/
            void OP_8xy5(){
                unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;
                unsigned short vyIndex = (opcode & 0x00F0u) >> 4u;

                registers[vxIndex] -= registers[vyIndex];

                if (registers[vxIndex] > 0xFFu) {
                    registers[0xF] = 0x00u;
                }
                else {
                    registers[0xF] = 0x01u;
                }
            }
            /*Store the value of register VY shifted right one bit in register VX¹
             * Set register VF to the least significant bit prior to the shift
             * VY is unchanged*/
            void OP_8xy6(){
                unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;
                unsigned short vyIndex = (opcode & 0x00F0u) >> 4u;

                registers[0xFu] = registers[vyIndex] & 1u;//get the least significant bit of VY and set VF to it
                registers[vxIndex] = registers[vyIndex] >> 1u;//set VX to VY shifted right one bit
            }
            /*Set register VX to the value of VY minus VX
             * Set VF to 00 if a borrow occurs
             * Set VF to 01 if a borrow does not occur*/
            void OP_8xy7(){
                unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;
                unsigned short vyIndex = (opcode & 0x00F0u) >> 4u;

                registers[vxIndex] = registers[vyIndex] - registers[vxIndex];

                if (registers[vxIndex] > 0xFFu) {//subtracting a smaller unsigned int from a larger unsigned int causes the number to loop to the largest number possible
                    registers[0xFu] = 0x00u;     //therefore, if a borrow occurs, VX will be much larger than 255 and this if statement checks for that
                }
                else {
                    registers[0xFu] = 0x01u;
                }
            }
            /*Store the value of register VY shifted left one bit in register VX¹
             * Set register VF to the most significant bit prior to the shift
             * VY is unchanged*/
            void OP_8xyE(){
                unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;
                unsigned short vyIndex = (opcode & 0x00F0u) >> 4u;

                registers[0xFu] = registers[vyIndex] >> 3u;//get the most significant bit of VY and set VF to it
                registers[vxIndex] = registers[vyIndex] << 1u;//set VX to VY shifted left one bit
            }

        //Stores A Pointer To The Two Instructions Beginning With: E
        void TableE(){
            ((*this).*(subTable8[opcode & 0x000Fu]))();
        }

        //Table E Functions
            //Skip the following instruction if the key corresponding to the hex value currently stored in register VX is not pressed
            void OP_ExA1(){
                unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;
                unsigned char vxValue = registers[vxIndex];

                if (keypad[vxValue] == 0) {
                    pc += 2;//increase program counter by 2 to skip the current instruction
                }           
            }
            //Skip the following instruction if the key corresponding to the hex value currently stored in register VX is pressed
            void OP_Ex9E(){
                unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;
                unsigned char vxValue = registers[vxIndex];

                if (keypad[vxValue] != 0) {
                    pc += 2;
                }
            }

        //Stores A Pointer To The Nine Instructions Beginning With: F
        void TableF(){
            ((*this).*(subTableF[opcode & 0x00FFu]))();
        }

        //Table F Functions
            //Store the current value of the delay timer in register VX
            void OP_Fx07() {
                unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;

                registers[vxIndex] = delayTimer;
            }
            //Wait for a keypress and store the result in register VX
            void OP_Fx0A(){
                unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;
                bool keyPressed = false;//Bool to determine if a key has been pressed

                while (!keyPressed) {//Loop will not exit until a key has been pressed
                    for (int i = 0; i < 16; ++i) {//Loop through each key on the keypad to see if it is pressed
                        if (keypad[i] != 0) {//If a key is pressed, it is stored in register VX and the for and while loops are exited
                            registers[vxIndex] = static_cast<unsigned char>(i);
                            keyPressed = true;
                            break;
                        }
                    }
                }
            }
            //Set the delay timer to the value of register VX
            void OP_Fx15(){
                unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;

                delayTimer = registers[vxIndex];
            }
            //Set the sound timer to the value of register VX
            void OP_Fx18(){
                unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;

                soundTimer = registers[vxIndex];
            }
            //Add the value stored in register VX to register I
            void OP_Fx1E(){
                unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;

                index += registers[vxIndex];
            }
            //Set I to the memory address of the sprite data corresponding to the hexadecimal digit stored in register VX
            void OP_Fx29(){
                unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;
                unsigned char vxValue = registers[vxIndex];//get the value stored in register VX

                index = FONTSET_START_ADDRESS + (vxValue * 5);//set the index register to the 5-byte sprite in the font set
            }
            //Store the binary-coded decimal equivalent of the value stored in register VX at addresses I (index), I + 1, and I + 2
            void OP_Fx33(){
                unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;

                unsigned char hundreds = registers[vxIndex] / 100;//Get the hundreds digit
                unsigned char tens = (registers[vxIndex] % 100) / 10;//Get the tens digit
                unsigned char units = registers[vxIndex] % 10;//Get the unit digit

                memory[index] = hundreds;//Places the digits in memory addresses
                memory[index + 1] = tens;
                memory[index + 2] = units;
            }
            /*Store the values of registers V0 to VX inclusive in memory starting at address I
            * I is set to I + X + 1 after operation²*/
            void OP_Fx55(){
                unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;

                for (unsigned int i = 0x0u; i <= vxIndex; i++) {//loop through and assign memory[index] to a register until VX is reached, then loops 1 more time and exits loop
                    memory[index] = registers[i];
                    index++;//increment index after each assignment
                }
            }
            /*Fill registers V0 to VX inclusive with the values stored in memory starting at address I
            * I is set to I + X + 1 after operation*/
            void OP_Fx65(){
                unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;

                for (unsigned int i = 0x0u; i <= vxIndex; i++) {//loop through and assign registers to memory[index] until VX is reached, then loops 1 more time and exits loop
                    registers[i] = memory[index];
                    index++;
                }
            }

        //Master Table Functions (Functions That Have Unique Identifiers)
        void OP_1nnn(){
        }
        //Execute subroutine starting at address NNN
        void OP_2nnn(){
            unsigned short address = opcode & 0x0FFFu;//get hexadecimal memory address nnn from the opcode and assign it to a variable
            stack[sp] = pc;//program counter is stored in the stack array so the subroutine can be returned from
            ++sp;
            pc = address;//set program counter to the obtained address
        }
        //Skip the following instruction if the value of register VX equals NN
        void OP_3xnn(){
            unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;
            unsigned char nn = opcode & 0x00FFu;//gets hexadecimal byte nn from the opcode and assigns it to a variable

            if (registers[vxIndex] == nn) {
                pc += 2;
            }
        }
        //Skip the following instruction if the value of register VX is not equal to NN
        void OP_4xnn(){
            unsigned short vxIndex = (opcode & 0x0F00u) >> 8u;
            unsigned char nn = opcode & 0x00FFu;

            if (registers[vxIndex] != nn) {
                pc += 2;
            }
        }
        //Skip the following instruction if the value of register VX is equal to the value of register VY
        void OP_5xy0(){
            unsigned short vxIndex = (opcode & 0x0FFFu) >> 8u;
            unsigned short vyIndex = (opcode & 0x0FFFu) >> 4u;

            if (registers[vxIndex] == registers[vyIndex]) {
                pc += 2;
            }
        }
        void OP_6xnn(){
        }
        void OP_7xnn(){
        }
        //Skip the following instruction if the value of register VX is not equal to the value of register VY
        void OP_9xy0(){
            unsigned short vxIndex = (opcode & 0x0FFFu) >> 8u;
            unsigned short vyIndex = (opcode & 0x0FFFu) >> 4u;

            if (registers[vxIndex] != registers[vyIndex]) {
                pc += 2;
            }
        }
        //Store memory address NNN in register I
        void OP_Annn(){
            unsigned short address = (opcode & 0x0FFFu);
            index = address;
        }
        //Jump to address NNN + V0
        void OP_Bnnn(){
            unsigned short address = opcode & 0x0FFFu;
            address += registers[0];
            pc = address;
        }
        //Set VX to a random number with a mask of NN
        void OP_Cxnn(){
            unsigned short vxIndex = (opcode & 0x0FFFu) >> 8u;
            unsigned char nn = opcode & 0x00FFu;

            registers[vxIndex] = (getRandom() & nn);

        }
        void OP_Dxyn(){
        }

        //Operation Not Found Function
        void OP_NULL(){
            throw NullOperationException("ERROR, The Operation: " + toHexString(opcode) + " Is Unrecognized");
        }
    };

    
    //Main Is For Testing Any Functions Of The Emulator We Will Comment It Out After Integrating It With The UML
    int main(){
        //Test Emulator Functions
        Chip8 myEmulator = Chip8();
        
        //Break The Instruction Into Two Halves (Just Like The Memory List Does)
        //Test Instruction 0nnn
        unsigned char firstDigits = 0x0F;
        unsigned char secondDigits = 0xFF;

        //Assign The First Availble Memory An Instruction
        myEmulator.memory[0x200] = firstDigits;
        myEmulator.memory[0x201] = secondDigits;

        //Try To Execute The Instruction
        try{
            myEmulator.nextInstruction();
        }catch (const std::exception& e){
            std::cout << "Caught exception: " << e.what() << "\n";
        }
    
        //Sound Test
        //Beep(440,500);
    }   
    
