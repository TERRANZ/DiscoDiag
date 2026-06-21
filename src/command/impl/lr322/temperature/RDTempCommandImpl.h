//
// Created by terranz on 27.02.25.
//

#ifndef RDTEMPCOMMANDIMPL_H
#define RDTEMPCOMMANDIMPL_H
#include "src/command/AbstractCommand.h"


class RDTempCommandImpl final : public AbstractCommand {
public:
    RDTempCommandImpl();

    int calculate(const QString &value) override;
};


#endif //RDTEMPCOMMANDIMPL_H
