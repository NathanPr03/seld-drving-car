#include "Command.cpp"

class CommandFactory {
public:
    Command* create(int direction){
        switch(direction) {
            case(TURN_LEFT_GRADUAL):
                return new DriveLeftCommand();
                break;
            case(TURN_RIGHT_GRADUAL):
                return new DriveRightCommand();
                break;
            case(TURN_RIGHT_HARD):
                return new DriveRightHardCommand();
                break;
            case(TURN_LEFT_HARD):
                return new DriveLeftHardCommand();
                break;
            case(STRAIGHT_AHEAD):
                return new DriveStraightCommand();
                break;
            default:
                return new DriveStraightCommand();
                break;
        }

    }
};