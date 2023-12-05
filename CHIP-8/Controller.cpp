/*
Author: Scott Field
Name: Application Loop
Date: 12/04/2023
Version: 0.1
Description:
Handle the functions for when an event (button press, key press) occurs in the emulator
*/
#include "Chip8.cpp"
class Controller{
    //Member Variable
    Chip8 emulator;

public:
    //Constructor
    Controller(Chip8 &emulator): emulator(emulator){}

    //methods

    void onKeyPressed(char keyVal){ 
    }

    void onKeyReleased(char keyVal){
    }

    void onPauseBtn(boolean isPaused){

    }

    boolean onOpenBtn(){
    }

    boolean onCloseBtn(){
    }

    boolean onExitBtn(){
    }

    void onChangeColor(){
    }

};