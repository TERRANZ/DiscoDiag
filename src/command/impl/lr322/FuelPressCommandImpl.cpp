//
// Created by terranz on 27.02.25.
//

#include "FuelPressCommandImpl.h"
#include <src/command/commandIds.h>
#include <src/obd/ObdParser.h>

FuelPressCommandImpl::FuelPressCommandImpl() : AbstractCommand(CMD_PRESS_FUEL, "Fuel Pressure Command")
{
}

int FuelPressCommandImpl::calculate(const QString &value)
{
    return ((ObdParser::A(value) * 256) + ObdParser::B(value)) / 100;
}
