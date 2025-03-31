//
// Created by terranz on 27.02.25.
//

#include "FuelLevelCommandImpl.h"
#include <src/command/commandIds.h>

FuelLevelCommandImpl::FuelLevelCommandImpl() : AbstractCommand(CMD_LVL_FUEL_LITRES, "Fuel Level Command")
{
}

int FuelLevelCommandImpl::calculate(const QString &value)
{
    return 0;
}
