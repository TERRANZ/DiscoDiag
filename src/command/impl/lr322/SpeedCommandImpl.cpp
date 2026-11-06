//
// Created by terranz on 06.11.26.
//

#include "SpeedCommandImpl.h"
#include "src/command/commandIds.h"
#include "src/obd/ObdParser.h"

SpeedCommandImpl::SpeedCommandImpl()
    : AbstractCommand(CMD_SPEED, "Vehicle speed command", &ObdResult::speed) {}

int SpeedCommandImpl::calculate(const QString &value) const {
    return ObdParser::A(value);
}
