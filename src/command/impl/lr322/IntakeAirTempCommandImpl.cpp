//
// Created by terranz on 27.02.25.
//

#include "IntakeAirTempCommandImpl.h"

#include "src/command/commandIds.h"
#include "src/obd/ObdParser.h"

IntakeAirTempCommandImpl::IntakeAirTempCommandImpl(): AbstractCommand(
    CMD_TEMP_AIR_INTAKE, "Intake air temp command") {
}

int IntakeAirTempCommandImpl::calculate(const QString &value) {
    return ObdParser::extractDigitA(value) - 40;
}
