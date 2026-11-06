//
// Created by terranz on 06.11.26.
//

#include "UAbsCommandImpl.h"
#include "src/command/commandIds.h"
#include "src/obd/ObdParser.h"

UAbsCommandImpl::UAbsCommandImpl()
    : AbstractCommand(CMD_U_ABS, "ABS control module voltage command",
                      &ObdResult::uAbs) {}

int UAbsCommandImpl::calculate(const QString &value) const {
    return ObdParser::A(value) * 20.4 / 255;
}
