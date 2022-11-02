#include "Command.h"

class CommandFactory {
public:
    Command* create(int direction);
};