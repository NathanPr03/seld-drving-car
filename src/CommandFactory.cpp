#include "Command.cpp"

class CommandFactory {
public:
    Command* create(int direction, int motorLeft[], int motorRight[]){
        switch(direction) {
            case(TURN_LEFT_GRADUAL):
                return new DriveLeftCommand(motorLeft, motorRight);
                break;
            case(TURN_RIGHT_GRADUAL):
                return new DriveRightCommand(motorLeft, motorRight);
                break;
            case(TURN_RIGHT_HARD):
                return new DriveRightHardCommand(motorLeft, motorRight);
                break;
            case(TURN_LEFT_HARD):
                return new DriveLeftHardCommand(motorLeft, motorRight);
                break;
            case(STRAIGHT_AHEAD):
                return new DriveStraightCommand(motorLeft, motorRight);
                break;
            default:
                return new DriveStraightCommand(motorLeft, motorRight);
                break;
        }

    }
};