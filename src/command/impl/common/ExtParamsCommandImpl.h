//
// Created by terranz on 25.02.25.
//

#ifndef EXTPARAMSCOMMANDIMPL_H
#define EXTPARAMSCOMMANDIMPL_H
#include "src/command/AbstractCommand.h"


class ExtParamsCommandImpl final : public AbstractCommand {
public:
    ExtParamsCommandImpl() : AbstractCommand("", "Extended params command") {
    }
};


#endif //EXTPARAMSCOMMANDIMPL_H
