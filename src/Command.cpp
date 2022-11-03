#include "MotorController.cpp"

class Command {
private:
    int motors_power[4];
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
    DriveStraightCommand(int motorLeft[], int motorRight[]){
        motorController = new MotorController(motorLeft, motorRight);
    }
    ~DriveStraightCommand(){}

    void execute() {
        motorController->drive(motorsPower);
    }

    void undo() {
        motorController->drive(reversedMotorsPower);
    }
};

class DriveRightCommand : public Command {
private:
    MotorController *motorController;
    int motorsPower[4] = {0, 0, 1, 0};
    int reversedMotorsPower[4] = {0, 1, 0, 0};
public:
    DriveRightCommand(int motorLeft[], int motorRight[]){
        motorController = new MotorController(motorLeft, motorRight);
    }
    ~DriveRightCommand(){}

    void execute() {
        motorController->drive(motorsPower);
    }

    void undo() {
        motorController->drive(reversedMotorsPower);
    }
};

class DriveRightHardCommand : public Command {
private:
    MotorController *motorController;
    int motorsPower[4] = {1, 0, 1, 0};
    int reversedMotorsPower[4] = {0, 1, 0, 1};
public:
    DriveRightHardCommand(int motorLeft[], int motorRight[]){
        motorController = new MotorController(motorLeft, motorRight);
    }
    ~DriveRightHardCommand(){}

    void execute() {
        motorController->drive(motorsPower);
    }

    void undo() {
        motorController->drive(reversedMotorsPower);
    }
};

class DriveLeftCommand : public Command {
private:
    MotorController *motorController;
    int motorsPower[4] = {0, 1, 0, 0};
    int reversedMotorsPower[4] = {0, 0, 1, 0};
public:
    DriveLeftCommand(int motorLeft[], int motorRight[]){
        motorController = new MotorController(motorLeft, motorRight);
    }
    void execute() {
        motorController->drive(motorsPower);
    }

    void undo() {
        motorController->drive(reversedMotorsPower);
    }
};

class DriveLeftHardCommand : public Command {
private:
    MotorController *motorController;
    int motorsPower[4] = {0, 1, 0, 1};
    int reversedMotorsPower[4] = {1, 0, 1, 0};
public:
    DriveLeftHardCommand(int motorLeft[], int motorRight[]){
        motorController = new MotorController(motorLeft, motorRight);
    }
    void execute() {
        motorController->drive(motorsPower);
    }

    void undo() {
        motorController->drive(reversedMotorsPower);
    }
};


