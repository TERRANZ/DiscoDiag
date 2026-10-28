//
// Created by terranz on 21.02.25.
//

#include "CoolantTempCommandImpl.h"
#include "src/command/commandIds.h"
#include "src/obd/ObdParser.h"

CoolantTempCommandImpl::CoolantTempCommandImpl() : AbstractCommand(CMD_TEMP_COOLANT, "Coolant temp command", &ObdResult::tempCoolant) {
}

int CoolantTempCommandImpl::calculate(const QString &value) const {
    return ObdParser::A(value) - 40;
}
