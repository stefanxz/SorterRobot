//
// Created by Stefan on 07.05.2024.
//


#include <iostream>
#include "DisplayController.h"

using namespace std;

int main()
{
    DisplayController display(0x27);
    display.display_setup();
    display.display_string("BBL Drizzy versus Kendrick Lebar");
    display.display_clear();
    return 0;
}