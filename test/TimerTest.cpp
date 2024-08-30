#include <cassert>
#include <iostream>
#include <chrono>
#include <thread>

#include "../include/Timer.hpp"

using namespace std;

void test_constructor() {
    Timer timer(0, 1, 0); // 1 minutes
    int timeRemaining = timer.remainingMilliseconds();
    assert(timeRemaining > 59995 && timeRemaining < 60001); // 5 Millisecond margin of error
    assert(timer.isRunning());
    cout << "test_constructor passed!\n";
}

/*
*   Sets a timer for 1 minute then checks if the pause and resume work 
*/
void test_pause_resume() {
    Timer timer(0, 1, 0); // One minute
    this_thread::sleep_for(chrono::seconds(1));
    timer.pause();
    int timeRemaining = timer.remainingMilliseconds();
    assert(!timer.isRunning());
    this_thread::sleep_for(chrono::seconds(1));
    assert(timer.remainingMilliseconds() == timeRemaining);
    timer.resume();
    assert(timer.isRunning());
    assert(timer.remainingMilliseconds() < 59010);
    std::cout << "test_pause_resume passed!\n";
}

/*
*   Tests that the reset reverts back to original time and is stopped
*/
void test_reset() {
    Timer timer(0, 1, 0);
    this_thread::sleep_for(chrono::seconds(1));
    timer.reset();
    assert(timer.remainingMilliseconds() == 60000);
    assert(!timer.isRunning());
    std::cout << "test_reset passed!\n";
}

/*
*   Tests adding time increases remaining time
*/
void test_add_time() {
    Timer timer(0, 1, 0);
    timer.addTime(30);
    int timeRemaining = timer.remainingMilliseconds();
    assert(timeRemaining > 89995 && timeRemaining < 90001);
    std::cout << "test_add_time passed!\n";
}

/*
*   Test percent elapsed for the progress bar
*/
void test_percent_elapsed() {
    Timer timer(0, 0, 4);
    this_thread::sleep_for(chrono::seconds(2));
    double percent = timer.percentElapsed();
    assert(percent > 49.0 && percent < 51.0); // allows for a bit or margin of error
    std::cout << "test_percent_elapsed passed!\n";
}

/*
*   Testing the timer stops when it finishes 
*/
void test_expiration() {
    Timer timer(0, 0, 2); // 2 seconds
    this_thread::sleep_for(chrono::seconds(3));
    assert(timer.remainingMilliseconds() == 0);
    assert(!timer.isRunning());
    cout << "test_expiration passed!\n";
}

int main() {
    try {
        test_constructor();
        test_pause_resume();
        test_reset();
        test_add_time();
        test_percent_elapsed();
        test_expiration();
        cout << "All tests passed successfully!\n";
    } catch (const exception& e) {
        cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

// void test_test(){
//     assert(true);
// }

// int main(){
//     try{
//     test_test();
//     } catch (const exception& e) {
//         std::cerr << "Test failed with exception: " << e.what() << std::endl;
//         return 1;
//     }
//     return 0;
    
// }
