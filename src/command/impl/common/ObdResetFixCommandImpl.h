#ifndef OBDRESETFIXCOMMANDIMPL_H
#define OBDRESETFIXCOMMANDIMPL_H
#include "src/command/AbstractCommand.h"

class ObdResetFixCommandImpl : public AbstractCommand {
public:
    ObdResetFixCommandImpl() : AbstractCommand("ATZ", "Obd Reset") {
    }
};

#endif // OBDRESETFIXCOMMANDIMPL_H
