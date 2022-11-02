class Command {
    private:
        int motors_power[4];
    public:
        virtual void execute() = 0;
        virtual void undo() = 0;
};
