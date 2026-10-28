//
// Created by terranz on 27.02.25.
//

#ifndef FUELPRESSCOMMANDIMPL_H
#define FUELPRESSCOMMANDIMPL_H
#include "src/command/AbstractCommand.h"

class FuelPressCommandImpl final : public AbstractCommand
{
public:
    FuelPressCommandImpl();

    int calculate(const QString &value) const override;
};

#endif // FUELPRESSCOMMANDIMPL_H
