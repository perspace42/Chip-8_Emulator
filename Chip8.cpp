/*
Author: Scott Field
Name: Prototype Zero Chip 8 Emulator
Version: 1.0
Date: 11/13/2023
Description:
Form The Framework For The Next Series Of Chip-8 Prototype Emulators To Be Built Off of
*/
#include <windows.h> //For Sound Emulation
#include <fstream> //For File Reading
#include <ctime> //For Random Number
#include <iostream> //For Printing Output When Testing

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
    typedef void (Chip8::*Chip8Table)();
    Chip8Table setNull(Chip8Table * table,int length){
        for (int i = 0; i < length; i++){
            table[i] = &OP_NULL;
        } 
        return *table;
    }

    //Chip-8 Random Number Function
    int getRandom(){
        //This seeds the random number generator using the current time
        srand(time(NULL));
        //Generate The Random Number (0 to 255)
        return int(rand() % 256);
    }

    //Public Class Methods
    public: 
        //Clear The Values Currently In The Emulator
        void clearEmulator(){
            //Clear All The Emulated Variables (By setting the memory within them to unsigned 0)
            memset(registers    ,   0u  , sizeof(registers)   );
            memset(memory       ,   0u  , sizeof(memory)      );
            memset(&index       ,   0u  , sizeof(index)       );
            memset(&pc          ,   0u  , sizeof(pc)          );
            memset(stack        ,   0u  , sizeof(stack)       );
            memset(&sp          ,   0u  , sizeof(sp)          );
            memset(&delayTimer  ,   0u  , sizeof(delayTimer)  );
            memset(&soundTimer  ,   0u  , sizeof(soundTimer)  );
            memset(keypad       ,   0u  , sizeof(keypad)      );
            memset(video        ,   0u  , sizeof(video)       );
            memset(&opcode      ,   0u  , sizeof(opcode)      );
        }

        //Load The Program From The File (This Needs Some Input Validation To Ensure That The File Is A Valid)
        void loadProgram(char const* filename){
            // Open the file as a stream of binary and move the file pointer to the end
            std::ifstream file(filename, std::ios::binary | std::ios::ate);

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
            }
        }

        //Execute The Next Instruction From The Program
        void nextInstruction(){
            //First Read The Two-Byte Opcode From Memory 
            opcode = (memory[pc] << 8u) | memory[(pc + 1)];
            //Second increment the program counter by 2
            pc += 2;
            //Decode The Opcode Using The Function Table To Determine Which Operation Needs To Be Performed
            ((*this).*(MASTER_TABLE[(opcode & 0xF000u) >> 12u]))(); 
            
            //If The Delay Timer Has Been Set, Decrement It
            if (delayTimer > 0){
                --delayTimer;
            }

            //If The Sound Timer Has Been Set, Decrement It
            if (soundTimer > 0){
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
            //set the first instruction as the next instruction to be executed
            pc = START_ADDRESS; 
            for (unsigned int i = 0; i < 80; ++i) 
                //loads the font into program memory
                memory[FONTSET_START_ADDRESS + i] = fontset[i];
            //get the first random number
            int randomNumber = getRandom();    

            //Overwrite The Command Not Found Function (OP_NULL) In The Sub Tables Where The Other Functions Need To Be Inserted

            //Sub Table 0 Overwrite
            subTable0[0x0] = &Chip8::OP_00E0;
		    subTable0[0xE] = &Chip8::OP_00EE;

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
            &OP_3xkk,
            &OP_4xkk,
            &OP_5xy0,
            &OP_6xkk,
            &OP_7xkk,
            &Table8,
            &OP_9xy0,
            &OP_Annn,
            &OP_Bnnn,
            &OP_Cxkk,
            &OP_Dxyn,
            &TableE,
            &TableF
        };
        //function pointer sub tables Intializes Their Values To Automatically Point To The Command Not Found Function (OP_NULL)
        Chip8Table subTable0[15] = {setNull(subTable0,15)};
        
        Chip8Table subTable8[15] = {setNull(subTable8,15)};

        Chip8Table subTableE[15] = {setNull(subTableE,15)};
        
        Chip8Table subTableF[102] = {setNull(subTableF,102)};

        //Instruction List Function Implementation
        
        //Stores A Pointer To The Two Instructions Beginning With: 00
        void Table0(){ 
            ((*this).*(subTable0[opcode & 0x000Fu]))();
        }

        //Table 0 Functions
            void OP_00E0(){
            }

            void OP_00EE(){  
            }

        //Stores A Pointer To The Nine Instructions Beginning With: 8
        void Table8(){
            ((*this).*(subTable8[opcode & 0x000Fu]))();
        }

        //Table 8 Functions
            void OP_8xy0(){
            }

            void OP_8xy1(){
            }

            void OP_8xy2(){
            }

            void OP_8xy3(){
            }

            void OP_8xy4(){
            }

            void OP_8xy5(){
            }

            void OP_8xy6(){
            }

            void OP_8xy7(){
            }

            void OP_8xyE(){
            }
        //Stores A Pointer To The Two Instructions Beginning With: E
        void TableE(){
            ((*this).*(subTable8[opcode & 0x000Fu]))();
        }

        //Table E Functions
            void OP_ExA1(){
            }

            void OP_Ex9E(){
            }

        //Stores A Pointer To The Nine Instructions Beginning With: F
        void TableF(){
            ((*this).*(subTableF[opcode & 0x00FFu]))();
        }

        //Table F Functions
            void OP_Fx07(){
            }
            void OP_Fx0A(){
            }
            void OP_Fx15(){
            }
            void OP_Fx18(){
            }
            void OP_Fx1E(){
            }
            void OP_Fx29(){
            }
            void OP_Fx33(){
            }
            void OP_Fx55(){
            }
            void OP_Fx65(){
            }

        //Master Table Functions (Functions That Have Unique Identifiers)
        void OP_1nnn(){
        }
        void OP_2nnn(){
        }
        void OP_3xkk(){
        }
        void OP_4xkk(){
        }
        void OP_5xy0(){
        }
        void OP_6xkk(){
        }
        void OP_7xkk(){
        }
        void OP_9xy0(){
        }
        void OP_Annn(){
        }
        void OP_Bnnn(){
        }
        void OP_Cxkk(){
        }
        void OP_Dxyn(){
        }

        //Operation Not Found Function
        void OP_NULL(){
            //Print Error Message
            std::cout << "Unknown Instruction: " << opcode;
            //Exit Entire Program (Show That A Problem Occurred)
            std::exit(EXIT_FAILURE);
        }
    };

    
    //Instruction List
    int main(){
        //Test Clear Emulator Function
        Chip8 myEmulator = Chip8();
    
        myEmulator.pc = 3;
        std::cout << "Program Counter: " << myEmulator.pc << "\n";

        myEmulator.clearEmulator();
        std::cout << "Cleared Program Counter: " << myEmulator.pc;
        //Sound Test
        //Beep(440,500);
    }   
