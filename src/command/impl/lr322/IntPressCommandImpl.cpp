//
// Created by terranz on 06.11.26.
//

#include "IntPressCommandImpl.h"
#include "src/command/commandIds.h"
#include "src/obd/ObdParser.h"

IntPressCommandImpl::IntPressCommandImpl()
    : AbstractCommand(CMD_INT_PRES, "Intake manifold pressure command",
                      &ObdResult::intPres) {}

int IntPressCommandImpl::calculate(const QString &value) const {
    return ObdParser::A(value);
}
