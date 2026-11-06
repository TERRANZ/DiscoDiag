//
// Created by terranz on 06.11.26.
//

#include "AbsFLCommandImpl.h"
#include "src/command/commandIds.h"
#include "src/obd/ObdParser.h"

AbsFLCommandImpl::AbsFLCommandImpl()
    : AbstractCommand(CMD_ABS_FL, "ABS speed FL command", &ObdResult::absFL) {}

int AbsFLCommandImpl::calculate(const QString &value) const {
    return ObdParser::A(value);
}
