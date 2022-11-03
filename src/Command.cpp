#include "MotorController.cpp"
#include "application.h"

class Command {
private:
    int motors_power[4]{};
public:
    virtual void execute() = 0;

    virtual void undo() = 0;
};

class DriveStraightCommand : public Command {
private:
    MotorController *motorController;
    int motorsPower[4] = {0, 1, 1, 0};
    int reversedMotorsPower[4] = {1, 0, 1, 0};
public:
    ~DriveStraightCommand()= default;

    void execute() override {
        motorController->drive(motorsPower);
    }

    void undo() override {
        Serial.println("UNDO FUNCTION IS CALLED");
        motorController->drive(reversedMotorsPower);
    }
};

class DriveRightCommand : public Command {
private:
    MotorController *motorController;
    int motorsPower[4] = {0, 0, 1, 0};
    int reversedMotorsPower[4] = {0, 1, 0, 0};
public:
    ~DriveRightCommand(){}

    void execute() override {
        motorController->drive(motorsPower);
    }

    void undo() override {
        Serial.println("UNDO FUNCTION IS CALLED");
        motorController->drive(reversedMotorsPower);
    }
};

class DriveRightHardCommand : public Command {
private:
    MotorController *motorController;
    int motorsPower[4] = {1, 0, 1, 0};
    int reversedMotorsPower[4] = {0, 1, 0, 1};
public:
    ~DriveRightHardCommand()= default;

    void execute() override {
        motorController->drive(motorsPower);
    }

    void undo() override {
        Serial.println("UNDO FUNCTION IS CALLED");
        motorController->drive(reversedMotorsPower);
    }
};

class DriveLeftCommand : public Command {
private:
    MotorController *motorController;
    int motorsPower[4] = {0, 1, 0, 0};
    int reversedMotorsPower[4] = {0, 0, 1, 0};
public:
    ~DriveLeftCommand()= default;

    void execute() override {
        motorController->drive(motorsPower);
    }

    void undo() override {
        Serial.println("UNDO FUNCTION IS CALLED");
        motorController->drive(reversedMotorsPower);
    }
};

class DriveLeftHardCommand : public Command {
private:
    MotorController *motorController;
    int motorsPower[4] = {0, 1, 0, 1};
    int reversedMotorsPower[4] = {1, 0, 1, 0};
public:
    ~DriveLeftHardCommand()= default;

    void execute() override {
        motorController->drive(motorsPower);
    }

    void undo() override {
        Serial.println("UNDO FUNCTION IS CALLED");
        motorController->drive(reversedMotorsPower);
    }
};


