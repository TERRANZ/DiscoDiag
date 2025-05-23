//
// Created by terranz on 27.02.25.
//


#include "OilTempCommandImpl.h"

#include <QList>

#include "src/command/commandIds.h"
#include "src/obd/ObdParser.h"

OilTempCommandImpl::OilTempCommandImpl(): AbstractCommand(CMD_TEMP_OIL, "Oil temp command") {
}

int OilTempCommandImpl::calculate(const QString &value) {
    return ObdParser::extractDigitA(value) - 40;
}
