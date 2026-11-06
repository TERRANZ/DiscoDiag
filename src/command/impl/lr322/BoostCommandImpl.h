//
// Created by terranz on 06.11.26.
//

#ifndef BOOSTCOMMANDIMPL_H
#define BOOSTCOMMANDIMPL_H
#include "src/command/AbstractCommand.h"


class BoostCommandImpl final : public AbstractCommand {
public:
    BoostCommandImpl();

    int calculate(const QString &value) const override;
};


#endif // BOOSTCOMMANDIMPL_H
