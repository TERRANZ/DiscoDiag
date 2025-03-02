//
// Created by terranz on 21.02.25.
//

#include "CoolantTempCommandImpl.h"
#include "src/command/commandIds.h"
#include "src/obd/ObdParser.h"

CoolantTempCommandImpl::CoolantTempCommandImpl() : AbstractCommand(CMD_TEMP_COOLANT, "Coolant temp command") {
}

QString CoolantTempCommandImpl::calculate(const QString &value) {
    return QString::number(ObdParser::extractDigitA(value) - 40);
}
