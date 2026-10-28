//
// Created by terranz on 27.02.25.
//

#include "OilLevelCommandImpl.h"
#include "src/command/commandIds.h"
#include "src/obd/ObdParser.h"

OilLevelCommandImpl::OilLevelCommandImpl() : AbstractCommand(CMD_LVL_OIL, "Oil level command", &ObdResult::lvlOil) {
}

int OilLevelCommandImpl::calculate(const QString &value) const {
    return ObdParser::A(value);
}
