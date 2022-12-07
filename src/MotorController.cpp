#include "enums.h"
#include "application.h"

class MotorController {
public:
    MotorController(int motorLeft[], int motorRight[]){
        _motorLeft[0] = motorLeft[0];
        _motorLeft[1] = motorLeft[1];
        _motorRight[0] = motorRight[0];
        _motorRight[1] = motorRight[1];
        
        for(int i = 0; i < 2; i++){
            pinMode(_motorLeft[i], OUTPUT);
            pinMode(_motorRight[i], OUTPUT);
        }   
    }
    int vehicleStatus;

    void drive(int motorPower[4]) {
        Serial.println("Drive has been called!");
        digitalWrite(_motorLeft[0], motorPower[0]);
        digitalWrite(_motorLeft[1], motorPower[1]);
        digitalWrite(_motorRight[0], motorPower[2]);
        digitalWrite(_motorRight[1], motorPower[3]);
        // digitalWrite(_motorLeft[1], HIGH);
        // digitalWrite(_motorLeft[0], LOW);

        // digitalWrite(_motorRight[1], LOW);
        // digitalWrite(_motorRight[0], HIGH);
    }

private:
    int _motorRight[2];
    int _motorLeft[2];
};