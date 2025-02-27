//
// Created by terranz on 21.02.25.
//

#include "CoolantTempCommandImpl.h"
#include "src/command/commandIds.h"
#include "src/obd/ObdParser.h"

CoolantTempCommandImpl::CoolantTempCommandImpl() : AbstractCommand(CMD_TEMP_COOLANT, "Coolant temp command") {
}

QString CoolantTempCommandImpl::calculate(const QString &value) {
    auto split = value.split('\n');
    auto header = split.at(0);
    auto temp1 = split.at(1);
    auto temp2 = split.at(2);
    return QString::number(ObdParser::extractDigitA(temp1) - 40);
}
