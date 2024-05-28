#include "../util/motor_control/MotorController.h"
#include "../util/servo_control//Servo.h"
#include "../util/light_sensor/ADCReader.h"
#include "../util/display_control/DisplayController.h"

#include <iostream>

using namespace std;

int main() {
    char command;
    bool direction = true; // true = forward, false = backward
    int speed = 25; // default speed

    MotorController motor(11, 13, 15);
    motor.setup();

    cout << "The default speed & direction of motor is LOW & Forward....." << endl;
    cout << "r-run s-stop f-forward b-backward l-low m-medium h-high e-exit" << endl;

    while (true) {
        cin >> command;

        switch (command) {
            case 'r':
                motor.run(direction, speed);
                break;
            case 's':
                motor.stop();
                break;
            case 'f':
                direction = true;
                motor.run(direction, speed);
                break;
            case 'b':
                direction = false;
                motor.run(direction, speed);
                break;
            case 'l':
                speed = 25;
                motor.changeSpeed(speed);
                break;
            case 'm':
                speed = 50;
                motor.changeSpeed(speed);
                break;
            case 'h':
                speed = 75;
                motor.changeSpeed(speed);
                break;
            case 'e':
                motor.stop();
                return 0;
            default:
                cout << "<<< wrong data >>>" << endl;
                cout << "please enter the defined data to continue....." << endl;
        }
    }
}