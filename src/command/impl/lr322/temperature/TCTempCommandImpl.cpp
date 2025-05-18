//
// Created by terranz on 27.02.25.
//

#include "TCTempCommandImpl.h"
#include "src/command/commandIds.h"
#include "src/obd/ObdParser.h"


TCTempCommandImpl::TCTempCommandImpl(): AbstractCommand(CMD_TEMP_TC, "Transfer case temp command") {
}

int TCTempCommandImpl::calculate(const QString &value) {
    return ObdParser::extractDigitA(value) - 40;
}
