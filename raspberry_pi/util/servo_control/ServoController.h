#ifndef SERVO_H
#define SERVO_H

#include <wiringPi.h>
#include <softPwm.h>
#include <iostream>
#include <unistd.h>

// Define the default piston time
#define PISTON_TIME 1500000

enum ServoState {
    PUSHING,
    PULLING,
    PAUSING,
    IDLE
};

class ServoController {
public:
    ServoController(int pin);
    ~ServoController();

    void movePiston();
    void pushPiston();
    void pullPiston();
    void pausePiston();
	ServoState getState() const;
private:
    int pin;
    ServoState state = IDLE;
    unsigned long nextActionTime = 0;
    unsigned long time = PISTON_TIME; // 1 second in microseconds
};

#endif // SERVO_H
