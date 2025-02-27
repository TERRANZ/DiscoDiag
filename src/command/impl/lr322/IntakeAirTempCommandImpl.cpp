//
// Created by terranz on 27.02.25.
//

#include "IntakeAirTempCommandImpl.h"

#include "src/command/commandIds.h"
#include "src/obd/ObdParser.h"

IntakeAirTempCommandImpl::IntakeAirTempCommandImpl(): AbstractCommand(
    CMD_TEMP_AIR_INTAKE, "Intake air temp command") {
}

QString IntakeAirTempCommandImpl::calculate(const QString &value) {
    const auto split = value.split('\n');
    const auto &temp1 = split.at(1);
    return QString::number(ObdParser::extractDigitA(temp1) - 40);
}
