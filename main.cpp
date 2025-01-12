#include "classes.h"
#include <iostream>
using namespace std;


int main() {

    
    try {
        Terminal terminal;
        terminal.run();
    }
    catch (const std::exception& e) {
        cerr << "Error: " << e.what() <<endl;
        return 1;
    }
    return 0;
}