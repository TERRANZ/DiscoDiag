//
// Created by terranz on 06.11.26.
//

#ifndef RDIFLOCKCOMMANDIMPL_H
#define RDIFLOCKCOMMANDIMPL_H
#include "src/command/AbstractCommand.h"


class RDifLockCommandImpl final : public AbstractCommand {
public:
    RDifLockCommandImpl();

    int calculate(const QString &value) const override;
};


#endif // RDIFLOCKCOMMANDIMPL_H
