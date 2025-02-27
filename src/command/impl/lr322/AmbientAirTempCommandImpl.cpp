//
// Created by terranz on 27.02.25.
//

#include "AmbientAirTempCommandImpl.h"
#include "src/command/commandIds.h"
#include "src/obd/ObdParser.h"

AmbientAirTempCommandImpl::AmbientAirTempCommandImpl() : AbstractCommand(
    CMD_TEMP_AIR_AMBIENT, "Ambient air temp command") {
}

QString AmbientAirTempCommandImpl::calculate(const QString &value) {
    const auto split = value.split('\n');
    const auto &temp1 = split.at(1);
    return QString::number(ObdParser::extractDigitA(temp1) - 40);
}
