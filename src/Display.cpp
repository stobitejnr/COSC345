#include <windows.h>

#include "../include/Display.hpp"

#define PADDING "  "

using namespace std;

Display::Display() {
    _splash = "";
    _asciiBuffer = "";
    _controlBuffer = "";

    _oldAscii = "";
    _oldControls= "";
    _oldSplash = "";
}


#define ASCII_HEIGHT 8

/* =========================================================
TERMINAL CLEARING FOR SEAMLESS FRAMES
========================================================= */

void Display::clearScreen() {
    string clearscreen = "\033[2J\033[H";

    _oldAscii = "";
    _oldControls = "";
    _oldSplash = "";

    fast_print(clearscreen);
}

void Display::setCursor(int row, int col) {
    string setcursor = "\033[";
    setcursor.append(to_string(row));
    setcursor.append(";");
    setcursor.append(to_string(col));
    setcursor.append("H");

    fast_print(setcursor);
}

void Display::setSplash(string str){
    _splash = str;
}



/* =========================================================
STAGE TIMER IN ASCII
========================================================= */


void Display::stageTimerDisplay(int hours, int minutes, int seconds, int tenths){

    string to_print = "";

    //Only show hours if over 1 left
    if(hours != 0){
        to_print += to_string(hours) + ":";
    }

    //Check if minutes need leading 0
    if(int(minutes/10)!=0){
        to_print += to_string(minutes) + ":";
    }
    else{
        to_print += "0" + to_string(minutes) + ":";
    }

    //Check if seconds need leading zero
    if(int(seconds/10)!=0){
        to_print += to_string(seconds);
    }
    else{
        to_print += "0" + to_string(seconds);
    }
    
    for(int i = 0; i < ASCII_HEIGHT; i++){
        string line;
        for(char ch : to_print){
            if(ch == ':'){
                line += font1.at(10)[i];
                line += PADDING;
            }
            else if(ch == '.'){
                line += font1.at(11)[i];
                line += PADDING;
            }
            else{
                line += font1.at(ch-'0')[i];
                line += PADDING;
            }
        }
        _asciiBuffer+=((line+"\n"));
    }
}


void Display::stageTimerControls(){
    _controlBuffer+=("\n");
    _controlBuffer+=("===========================================\n");
    _controlBuffer+=("Control your timer with the following keys: \n");
    _controlBuffer+=("===========================================\n");
    _controlBuffer+=("\n");
    _controlBuffer+=("S : Start/Pause your timer.\n");
    _controlBuffer+=("R : Reset your timer.\n");
    _controlBuffer+=("I : Add 10 seconds to your timer.\n");
    _controlBuffer+=("Q : End your timer immediately and return to menu.\n");
    _controlBuffer+=("\n");

}

/* =========================================================
STAGE STOPWATCH IN ASCII
========================================================= */

void Display::stageStopwatchDisplay(int hours, int minutes, int seconds, int hundredths){

    string to_print = "";

    //Only show hours if over 1 left
    if(hours != 0){
        to_print += to_string(hours) + ":";
    }

    //Check if minutes need leading 0
    if(int(minutes/10)!=0){
        to_print += to_string(minutes) + ":";
    }
    else{
        to_print += "0" + to_string(minutes) + ":";
    }

    //Check if seconds need leading zero
    if(int(seconds/10)!=0){
        to_print += to_string(seconds);
    }
    else{
        to_print += "0" + to_string(seconds);
    }

    if(hours == 0){
        //Check if millis need leading zero
        to_print += ".";
        if(int(hundredths/10)!=0){
            to_print += to_string(hundredths);
        }
        else{
            to_print += "0" + to_string(hundredths);
        }
    }
    

    for(int i = 0; i < ASCII_HEIGHT; i++){
        string line;
        for(char ch : to_print){
            if(ch == ':'){
                line += font1.at(10)[i];
                line += PADDING;
            }
            else if(ch == '.'){
                line += font1.at(11)[i];
                line += PADDING;
            }
            else{
                line += font1.at(ch-'0')[i];
                line += PADDING;
            }
        }
        _asciiBuffer+=((line+"\n"));
    }
}

void Display::stageStopwatchControls(){
    _controlBuffer+=("\n");
    _controlBuffer+=("===========================================\n");
    _controlBuffer+=("Control your stopwatch with the following keys: \n");
    _controlBuffer+=("===========================================\n");
    _controlBuffer+=("\n");
    _controlBuffer+=("S : Start/Stop your stopwatch.\n");
    _controlBuffer+=("R : Reset your stopwatch.\n");
    _controlBuffer+=("A : Create a split at the current time.\n");
    _controlBuffer+=("Q : Stop your stopwatch immediately and return to menu.\n");
    _controlBuffer+=("\n");

}

void Display::printAscii(){
    if(_asciiBuffer != _oldAscii){
        setCursor(1,1);
        fast_print(_asciiBuffer);
        _oldAscii = _asciiBuffer;
    }
    _asciiBuffer = "";
}

void Display::printControls(){
    if(_controlBuffer != _oldControls){
        setCursor(9,1);
        fast_print(_controlBuffer);
        _oldControls = _controlBuffer;
    }
    _controlBuffer = "";
}

void Display::printSplash(){
    if(_splash != _oldSplash){
        setCursor(20,1);
        string clearline = "\033[2K";
        fast_print(clearline);
        setCursor(20,1);
        fast_print(_splash);
        _oldSplash = _splash;
    }
    _splash = "";
}

/* =========================================================
PARSING TIMER, CALLING PRINT FUNCTION
========================================================= */

void Display::tickTimer(Timer& timer){
    int remaining = timer.remainingMilliseconds();
    
    int hours = remaining / 3600000;
    remaining %= 3600000;
    int minutes = remaining / 60000;
    remaining %= 60000;
    int seconds = remaining / 1000;
    remaining %= 1000;
    int tenths = remaining / 100;



    stageTimerDisplay(hours, minutes, seconds, tenths);
    stageTimerControls();

    printAscii();
    printControls();
    printSplash();

}

void Display::tickStopwatch(Stopwatch& stopwatch){
    int milliseconds = stopwatch.currentMilliseconds();
    
    int hours = milliseconds / 3600000;
    milliseconds %= 3600000;
    int minutes = milliseconds / 60000;
    milliseconds %= 60000;
    int seconds = milliseconds / 1000;
    milliseconds %= 1000;
    int hundredths = milliseconds / 10;

    stageStopwatchDisplay(hours, minutes, seconds, hundredths);
    stageStopwatchControls();

    printAscii();
    printControls();
    printSplash();
}

template<typename char_type>
auto Display::fast_print(const std::basic_string<char_type>& sss) -> void
{
   HANDLE const output_handle = GetStdHandle(STD_OUTPUT_HANDLE);
   const auto char_count = static_cast<DWORD>(sss.length());
   if constexpr (std::is_same_v<char_type, char>)
      WriteConsoleA(output_handle, sss.c_str(), char_count, nullptr, nullptr);
   else
      WriteConsoleW(output_handle, sss.c_str(), char_count, nullptr, nullptr);
}