#ifndef DISCODIAG_DISPLAYHEADERCOMMANDIMPL_H
#define DISCODIAG_DISPLAYHEADERCOMMANDIMPL_H

#include "src/command/AbstractCommand.h"

class DisplayHeaderCommandImpl : public AbstractCommand {
public:
    DisplayHeaderCommandImpl() : AbstractCommand("ATH1", "Set display headers parameter.") {
    }
};

#endif // DISCODIAG_DISPLAYHEADERCOMMANDIMPL_H
