//
// Created by terranz on 06.11.26.
//

#include "AbsFRCommandImpl.h"
#include "src/command/commandIds.h"
#include "src/obd/ObdParser.h"

AbsFRCommandImpl::AbsFRCommandImpl()
    : AbstractCommand(CMD_ABS_FR, "ABS speed FR command", &ObdResult::absFR) {}

int AbsFRCommandImpl::calculate(const QString &value) const {
    return ObdParser::A(value);
}
