//
// Created by terranz on 06.11.26.
//

#include "AbsRRCommandImpl.h"
#include "src/command/commandIds.h"
#include "src/obd/ObdParser.h"

AbsRRCommandImpl::AbsRRCommandImpl()
    : AbstractCommand(CMD_ABS_RR, "ABS speed RR command", &ObdResult::absRR) {}

int AbsRRCommandImpl::calculate(const QString &value) const {
    return ObdParser::A(value);
}
