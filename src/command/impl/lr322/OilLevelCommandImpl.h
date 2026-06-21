//
// Created by terranz on 27.02.25.
//

#ifndef OILLEVELCOMMANDIMPL_H
#define OILLEVELCOMMANDIMPL_H
#include "src/command/AbstractCommand.h"


class OilLevelCommandImpl final : public AbstractCommand {
public:
    OilLevelCommandImpl();

    int calculate(const QString &value) override;
};


#endif //OILLEVELCOMMANDIMPL_H
