//
// Created by terranz on 06.11.26.
//

#include "HeightRLCommandImpl.h"
#include "src/command/commandIds.h"
#include "src/obd/ObdParser.h"

HeightRLCommandImpl::HeightRLCommandImpl()
    : AbstractCommand(CMD_HEIGHT_RL, "Suspension height RL command",
                      &ObdResult::heightRL) {}

int HeightRLCommandImpl::calculate(const QString &value) const {
    return ((ObdParser::A(value) * 256) + ObdParser::B(value)) / 10 - 20;
}
