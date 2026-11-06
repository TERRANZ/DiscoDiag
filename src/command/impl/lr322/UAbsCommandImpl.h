//
// Created by terranz on 06.11.26.
//

#ifndef UABSCOMMANDIMPL_H
#define UABSCOMMANDIMPL_H
#include "src/command/AbstractCommand.h"


class UAbsCommandImpl final : public AbstractCommand {
public:
    UAbsCommandImpl();

    int calculate(const QString &value) const override;
};


#endif // UABSCOMMANDIMPL_H
