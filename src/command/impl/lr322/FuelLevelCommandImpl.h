//
// Created by terranz on 27.02.25.
//

#ifndef FUELLEVELCOMMANDIMPL_H
#define FUELLEVELCOMMANDIMPL_H
#include "src/command/AbstractCommand.h"


class FuelLevelCommandImpl final : public AbstractCommand {
    FuelLevelCommandImpl();

    int calculate(const QString &value) override;
};


#endif //FUELLEVELCOMMANDIMPL_H
