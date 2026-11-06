//
// Created by terranz on 06.11.26.
//

#include "BoostCommandImpl.h"
#include "src/command/commandIds.h"
#include "src/obd/ObdParser.h"

BoostCommandImpl::BoostCommandImpl()
    : AbstractCommand(CMD_BOOST, "Boost command", &ObdResult::boost) {}

int BoostCommandImpl::calculate(const QString &value) const {
    return ObdParser::A(value);
}
