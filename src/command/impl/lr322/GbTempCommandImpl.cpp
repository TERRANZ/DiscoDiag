//
// Created by terranz on 27.02.25.
//

#include "GbTempCommandImpl.h"
#include "src/command/commandIds.h"
#include "src/obd/ObdParser.h"

GbTempCommandImpl::GbTempCommandImpl() : AbstractCommand(
    CMD_TEMP_GB, "Gearbox temp command") {
}

int GbTempCommandImpl::calculate(const QString &value) {
    return ObdParser::extractDigitA(value) - 40;
}
