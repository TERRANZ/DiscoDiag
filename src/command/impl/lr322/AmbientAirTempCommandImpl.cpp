//
// Created by terranz on 27.02.25.
//

#include "AmbientAirTempCommandImpl.h"
#include "src/command/commandIds.h"
#include "src/obd/ObdParser.h"

AmbientAirTempCommandImpl::AmbientAirTempCommandImpl() : AbstractCommand(
    CMD_TEMP_AIR_AMBIENT, "Ambient air temp command") {
}

int AmbientAirTempCommandImpl::calculate(const QString &value) {
    return ObdParser::extractDigitA(value) - 40;
}
