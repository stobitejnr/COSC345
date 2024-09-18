#include "../include/Menu.hpp"

using namespace std;

/* =========================================================
MAIN FUNCTION
========================================================= */

/**
 * @brief The main function of the program.
 * 
 * This function initializes the Menu object and starts the menu system. After the menu system finishes,
 * the program prints a quit message and terminates.
 * 
 * @return int Returns 0 upon successful execution.
 */
int main() {
    // Create a Menu object
    Menu menu(false);
    
    // Start the main menu system
    menu.start();
    
    menu.mainMenu();

    cout << "<< PROGRAM QUIT >>" << endl;

    return 0;
}
