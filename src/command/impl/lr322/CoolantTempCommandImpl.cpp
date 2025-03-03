//
// Created by terranz on 21.02.25.
//

#include "CoolantTempCommandImpl.h"
#include "src/command/commandIds.h"
#include "src/obd/ObdParser.h"

CoolantTempCommandImpl::CoolantTempCommandImpl() : AbstractCommand(CMD_TEMP_COOLANT, "Coolant temp command") {
}

int CoolantTempCommandImpl::calculate(const QString &value) {
    return ObdParser::extractDigitA(value) - 40;
}
