//
// Created by terranz on 27.02.25.
//

#include "GbTempCommandImpl.h"
#include "src/command/commandIds.h"
#include "src/obd/ObdParser.h"

GbTempCommandImpl::GbTempCommandImpl() : AbstractCommand(
    CMD_TEMP_GB, "Gearbox temp command") {
}

QString GbTempCommandImpl::calculate(const QString &value) {
    const auto split = value.split('\n');
    const auto &temp1 = split.at(1);
    return QString::number(ObdParser::extractDigitA(temp1) - 40);
}
