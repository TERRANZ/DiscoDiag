//
// Created by terranz on 06.11.26.
//

#ifndef INTPRESSCOMMANDIMPL_H
#define INTPRESSCOMMANDIMPL_H
#include "src/command/AbstractCommand.h"


class IntPressCommandImpl final : public AbstractCommand {
public:
    IntPressCommandImpl();

    int calculate(const QString &value) const override;
};


#endif // INTPRESSCOMMANDIMPL_H
