//
// Created by terranz on 06.11.26.
//

#ifndef SPEEDCOMMANDIMPL_H
#define SPEEDCOMMANDIMPL_H
#include "src/command/AbstractCommand.h"


class SpeedCommandImpl final : public AbstractCommand {
public:
    SpeedCommandImpl();

    int calculate(const QString &value) const override;
};


#endif // SPEEDCOMMANDIMPL_H
