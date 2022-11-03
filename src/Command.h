// class Command {
//     private:
//         int motors_power[4];
//     public:
//         virtual ~Command();
//         virtual void execute() = 0;
//         virtual void undo() = 0;
// };

// class DriveStraightCommand : public Command {
// public:
//     ~DriveStraightCommand();
//     void execute();
//     void undo();
// };

// class DriveRightCommand : public Command {
// public:
//     ~DriveRightCommand();
//     void execute();
//     void undo();
// };

// class DriveRightHardCommand : public Command {
// public:
//     ~DriveRightHardCommand();
//     void execute();
//     void undo();
// };

// class DriveLeftCommand : public Command {
// public:
//     ~DriveLeftCommand();
//     void execute();
//     void undo();
// };

// class DriveLeftHardCommand : public Command {
// public:
//     ~DriveLeftHardCommand();
//     void execute();
//     void undo();
// };