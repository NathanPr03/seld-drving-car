#include "enums.h"
#include "application.h"

class MotorController {
private:
    int motorLeft[2] = {A4, A5};
    int motorRight[2] = {D6, D5};

    __attribute__((unused)) void drive_backward(int duration) {
        Serial.println("Driving backward");

        digitalWrite(motorLeft[0], HIGH);
        digitalWrite(motorLeft[1], LOW);

        digitalWrite(motorRight[0], LOW);
        digitalWrite(motorRight[1], HIGH);

        delay(duration);
    }

public:
    ~MotorController()= default;
    int vehicleStatus;

    void drive(int motorPower[4]) {
        Serial.println("Drive has been called!");
        // digitalWrite(motorLeft[0], motorPower[0]);
        // digitalWrite(motorLeft[1], motorPower[1]);
        // digitalWrite(motorRight[0], motorPower[2]);
        // digitalWrite(motorRight[1], motorPower[3]);
        digitalWrite(motorLeft[0], HIGH);
        digitalWrite(motorLeft[1], HIGH);
        digitalWrite(motorRight[0], HIGH);
        digitalWrite(motorRight[1], HIGH);
    }
};