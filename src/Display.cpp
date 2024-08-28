#include <windows.h>
#include "../include/Display.hpp"

#define PADDING "  "
#define ASCII_HEIGHT 8

using namespace std;

/* =========================================================
CONSTRUCTOR
========================================================= */

/**
 * @class Display
 * @brief Handles the terminal display for a timer and stopwatch, including ASCII art and controls.
 */
Display::Display()
{
    _splash = "";
    _asciiBuffer = "";
    _barBuffer = "";
    _controlBuffer = "";
    _splitBuffer = "";

    _oldAscii = "";
    _oldControls = "";
    _oldSplash = "";
    _oldSplits = "";

    _asciiWidth = 0;
}

/* =========================================================
TERMINAL INTERACTION HELPERS
========================================================= */

/**
 * @brief Clears the terminal screen and resets the old buffers.
 */
void Display::clearScreen()
{
    string clearscreen = "\033[2J\033[H";

    _oldAscii = "";
    _oldControls = "";
    _oldSplash = "";
    _oldBar = "";

    fast_print(clearscreen);
}

/**
 * @brief Sets the cursor position in the terminal.
 * @param row The row position.
 * @param col The column position.
 */
void Display::setCursor(int row, int col)
{
    string setcursor = "\033[" + to_string(row) + ";" + to_string(col) + "H";
    fast_print(setcursor);
}

/**
 * @brief Clears a specific line in the terminal.
 * @param row The row number to clear.
 */
void Display::clearLine(int row)
{
    string clearline = "\033[K";
    setCursor(row, 1);
    fast_print(clearline);
}

/* =========================================================
SPLASH TEXT HELPERS
========================================================= */

/**
 * @brief Sets the splash text to be displayed at the top or bottom of the screen.
 * @param str The splash text to display.
 */
void Display::setSplash(string str)
{
    _splash = "<< " + str + " >>";
}

/**
 * @brief Clears the splash text from the screen.
 */
void Display::clearSplash()
{
    _splash = "";
}

/* =========================================================
STAGE TIMER IN ASCII
========================================================= */

/**
 * @brief Converts the timer values into an ASCII representation and stages it for display.
 * @param hours The number of hours.
 * @param minutes The number of minutes.
 * @param seconds The number of seconds.
 * @param tenths The number of tenths of a second.
 */
void Display::stageTimerDisplay(int hours, int minutes, int seconds, int tenths)
{
    string to_print = "";

    // Only display hours if there is at least 1 hour left
    if (hours != 0)
    {
        to_print += to_string(hours) + ":";
    }

    // Add leading zero to minutes if needed
    if (int(minutes / 10) != 0)
    {
        to_print += to_string(minutes) + ":";
    }
    else
    {
        to_print += "0" + to_string(minutes) + ":";
    }

    // Add leading zero to seconds if needed
    if (int(seconds / 10) != 0)
    {
        to_print += to_string(seconds);
    }
    else
    {
        to_print += "0" + to_string(seconds);
    }

    // Build the ASCII art for the timer display
    for (int i = 0; i < ASCII_HEIGHT; i++)
    {
        string line;
        for (char ch : to_print)
        {
            if (ch == ':')
            {
                line += font1.at(10)[i]; // Colon character
                line += PADDING;
            }
            else if (ch == '.')
            {
                line += font1.at(11)[i]; // Period character
                line += PADDING;
            }
            else
            {
                line += font1.at(ch - '0')[i]; // Numeric character
                line += PADDING;
            }
        }
        _asciiWidth = line.length(); // Update ASCII width
        _asciiBuffer += (line + "\n");
    }
}

/**
 * @brief Creates a visual progress bar for the timer based on the percentage completed.
 * @param percentage The percentage of completion.
 */
void Display::stageTimerBar(double percentage)
{
    int width = _asciiWidth - 2;
    int filled = (int)((percentage / 100) * width);

    string border = "";

    // Build the top and bottom borders of the bar
    border += "+";
    for (int i = 0; i < width; ++i)
    {
        border += "-";
    }
    border += "+\n";

    _barBuffer += border;

    _barBuffer += "|";

    // Fill the bar with '#' characters based on the percentage completed
    for (int i = 0; i < width; ++i)
    {
        if (i < filled)
        {
            _barBuffer += "#";
        }
        else
        {
            _barBuffer += " ";
        }
    }

    _barBuffer += "|\n";
    _barBuffer += border;
}

/**
 * @brief Stages the controls for the timer to be displayed in the terminal.
 */
void Display::stageTimerControls()
{
    _controlBuffer += "\n";
    _controlBuffer += "=======================================================\n";
    _controlBuffer += "S: Start/Pause | R: Reset | A: New Timer | Q: Main Menu \n";
    _controlBuffer += "=======================================================\n";
    _controlBuffer += "\n";
    // _controlBuffer += "S : Start/Pause your timer.\n";
    // _controlBuffer += "R : Reset your timer.\n";
    // _controlBuffer += "I : Add 10 seconds to your timer.\n";
    // _controlBuffer += "C : Change increment time.\n";
    // _controlBuffer += "Q : End your timer immediately and return to menu.\n";
    // _controlBuffer += "\n";
}

/* =========================================================
STAGE STOPWATCH IN ASCII
========================================================= */

/**
 * @brief Converts the stopwatch values into an ASCII representation and stages it for display.
 * @param hours The number of hours.
 * @param minutes The number of minutes.
 * @param seconds The number of seconds.
 * @param hundredths The number of hundredths of a second.
 */
void Display::stageStopwatchDisplay(int hours, int minutes, int seconds, int hundredths)
{
    string to_print = "";

    // Only show hours if there are more than 0
    if (hours != 0)
    {
        to_print += to_string(hours) + ":";
    }

    // Show minutes with a leading zero if needed
    if (hours)
    {
        if (int(minutes / 10) != 0)
        {
            to_print += to_string(minutes) + ":";
        }
        else
        {
            to_print += "0" + to_string(minutes) + ":";
        }
    }
    else if (minutes)
    {
        to_print += to_string(minutes) + ":";
    }

    // Show seconds with a leading zero if needed
    if (hours || minutes)
    {
        if (int(seconds / 10) != 0)
        {
            to_print += to_string(seconds);
        }
        else
        {
            to_print += "0" + to_string(seconds);
        }
    }
    else
    {
        to_print += to_string(seconds);
    }

    // Show hundredths of a second if there are no hours
    if (hours == 0)
    {
        to_print += ".";
        if (int(hundredths / 10) != 0)
        {
            to_print += to_string(hundredths);
        }
        else
        {
            to_print += "0" + to_string(hundredths);
        }
    }

    // Build the ASCII art for the stopwatch display
    for (int i = 0; i < ASCII_HEIGHT; i++)
    {
        string line;
        for (char ch : to_print)
        {
            if (ch == ':')
            {
                line += font1.at(10)[i]; // Colon character
                line += PADDING;
            }
            else if (ch == '.')
            {
                line += font1.at(11)[i]; // Period character
                line += PADDING;
            }
            else
            {
                line += font1.at(ch - '0')[i]; // Numeric character
                line += PADDING;
            }
        }
        _asciiBuffer += (line + "\n");
    }
}

/**
 * @brief Stages the controls for the stopwatch to be displayed in the terminal.
 */
void Display::stageStopwatchControls(){
    _controlBuffer+=("\n");
    _controlBuffer+=("======================================================\n");
    _controlBuffer+=("S: Start/Stop | R: Reset | A: Lap/Split | Q: Main Menu\n");
    _controlBuffer+=("======================================================\n");
    _controlBuffer+=("\n");
    // _controlBuffer+=("S : Start/Stop your stopwatch.\n");
    // _controlBuffer+=("R : Reset your stopwatch.\n");
    // _controlBuffer+=("A : Create a split at the current time.\n");
    // _controlBuffer+=("Q : Stop your stopwatch immediately and return to menu.\n");
    // _controlBuffer+=("\n");
}

void Display::stageStopwatchSplits(vector<int> splits){
    if(!splits.empty()){
        _splitBuffer+= "-+=| SPLITS |=+-\n";
        int i;
        if (splits.size()>10){ i = splits.size()-10; }
        else { i = 0; }
        for(i; i<splits.size(); ++i){

            int milliseconds = splits.at(i);
            int hours = milliseconds / 3600000;
            milliseconds %= 3600000;
            int minutes = milliseconds / 60000;
            milliseconds %= 60000;
            int seconds = milliseconds / 1000;
            milliseconds %= 1000;
            int hundredths = milliseconds / 10;
            string to_print = "";

            // Only show hours if there are more than 0
            if (hours != 0)
            {
                to_print += to_string(hours) + ":";
            }

            // Show minutes with a leading zero if needed
            if (hours)
            {
                if (int(minutes / 10) != 0)
                {
                    to_print += to_string(minutes) + ":";
                }
                else
                {
                    to_print += "0" + to_string(minutes) + ":";
                }
            }
            else if (minutes)
            {
                to_print += to_string(minutes) + ":";
            }

            // Show seconds with a leading zero if needed
            if (hours || minutes)
            {
                if (int(seconds / 10) != 0)
                {
                    to_print += to_string(seconds);
                }
                else
                {
                    to_print += "0" + to_string(seconds);
                }
            }
            else
            {
                to_print += to_string(seconds);
            }

            // Show hundredths of a second if there are no hours
            if (hours == 0)
            {
                to_print += ".";
                if (int(hundredths / 10) != 0)
                {
                    to_print += to_string(hundredths);
                }
                else
                {
                    to_print += "0" + to_string(hundredths);
                }
            }
            string fullprint = "Split " + to_string(i+1) + ": " + to_print + "\n";
            _splitBuffer += fullprint;
        }
    }
    else{
        _splitBuffer = "";
    }
}

/* =========================================================
PRINTING TO TERMINAL
========================================================= */

/**
 * @brief Prints the ASCII art representation of the timer or stopwatch to the terminal.
 * @param row The row position to print.
 */
void Display::printAscii(int row)
{
    if (_asciiBuffer != _oldAscii)
    {
        setCursor(row, 1);
        fast_print(_asciiBuffer);
        _oldAscii = _asciiBuffer;
    }
    _asciiBuffer = "";
}

/**
 * @brief Prints the controls for the timer or stopwatch to the terminal.
 * @param row The row position to print.
 */
void Display::printControls(int row)
{
    if (_controlBuffer != _oldControls)
    {
        setCursor(row, 1);
        fast_print(_controlBuffer);
        _oldControls = _controlBuffer;
    }
    _controlBuffer = "";
}

/**
 * @brief Prints the progress bar for the timer to the terminal.
 * @param row The row position to print.
 */
void Display::printBar(int row)
{
    if (_barBuffer != _oldBar)
    {
        setCursor(row, 1);
        fast_print(_barBuffer);
        _oldBar = _barBuffer;
    }
    _barBuffer = "";
}

/**
 * @brief Prints the splash text to the terminal.
 * @param row The row position to print.
 */
void Display::printSplash(int row)
{
    if (_splash != _oldSplash)
    {
        clearLine(row);
        setCursor(row, 1);
        fast_print(_splash);
        _oldSplash = _splash;
    }
}

void Display::printSplits(int row)
{
    if (_splitBuffer != _oldSplits)
    {
        setCursor(row, 1);
        fast_print(_splitBuffer);
        _oldSplits = _splitBuffer;
    }
    _splitBuffer = "";
}

/* =========================================================
PARSING TIMER, CALLING PRINT FUNCTION
========================================================= */

/**
 * @brief Updates the timer display on each tick, converting time into ASCII and updating the terminal.
 * @param timer The Timer object to get the remaining time from.
 */
void Display::tickTimer(Timer &timer)
{
    int remaining = timer.remainingMilliseconds();
    int hours = remaining / 3600000;
    remaining %= 3600000;
    int minutes = remaining / 60000;
    remaining %= 60000;
    int seconds = remaining / 1000;
    remaining %= 1000;
    int tenths = remaining / 100;

    stageTimerDisplay(hours, minutes, seconds, tenths);
    stageTimerBar(timer.percentElapsed());
    stageTimerControls();

    printAscii(1);
    printBar(10);
    printControls(13);
    printSplash(18);

    setCursor(20,1);
}

/**
 * @brief Updates the stopwatch display on each tick, converting time into ASCII and updating the terminal.
 * @param stopwatch The Stopwatch object to get the current time from.
 */
void Display::tickStopwatch(Stopwatch &stopwatch)
{
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
    stageStopwatchSplits(stopwatch.getSplits());

    printAscii(1);
    printControls(9);
    printSplash(14);
    printSplits(16);
    setCursor(15,1);
}

/* =========================================================
FAST PRINTING HELPER
========================================================= */

/**
 * @brief Prints a string to the console efficiently using the Windows API.
 * @tparam char_type The character type (either char or wchar_t).
 * @param sss The string to print.
 */
template <typename char_type>
auto Display::fast_print(const std::basic_string<char_type> &sss) -> void
{
    HANDLE const output_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    const auto char_count = static_cast<DWORD>(sss.length());
    if constexpr (std::is_same_v<char_type, char>)
        WriteConsoleA(output_handle, sss.c_str(), char_count, nullptr, nullptr);
    else
        WriteConsoleW(output_handle, sss.c_str(), char_count, nullptr, nullptr);
}
