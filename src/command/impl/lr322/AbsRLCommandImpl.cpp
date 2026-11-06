//
// Created by terranz on 06.11.26.
//

#include "AbsRLCommandImpl.h"
#include "src/command/commandIds.h"
#include "src/obd/ObdParser.h"

AbsRLCommandImpl::AbsRLCommandImpl()
    : AbstractCommand(CMD_ABS_RL, "ABS speed RL command", &ObdResult::absRL) {}

int AbsRLCommandImpl::calculate(const QString &value) const {
    return ObdParser::A(value);
}
