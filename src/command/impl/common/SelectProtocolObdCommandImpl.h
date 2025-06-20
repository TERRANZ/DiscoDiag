#ifndef SELECTPROTOCOLOBDCOMMANDIMPL_H
#define SELECTPROTOCOLOBDCOMMANDIMPL_H

#include "src/command/AbstractCommand.h"

class SelectProtocolObdCommandImpl : public AbstractCommand {
public:
    SelectProtocolObdCommandImpl() : AbstractCommand("ATSP6", "Select Protocol Command") {
    }
};

#endif //SELECTPROTOCOLOBDCOMMANDIMPL_H
