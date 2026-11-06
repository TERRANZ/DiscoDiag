//
// Created by terranz on 06.11.26.
//

#include "HeightFRCommandImpl.h"
#include "src/command/commandIds.h"
#include "src/obd/ObdParser.h"

HeightFRCommandImpl::HeightFRCommandImpl()
    : AbstractCommand(CMD_HEIGHT_FR, "Suspension height FR command",
                      &ObdResult::heightFR) {}

int HeightFRCommandImpl::calculate(const QString &value) const {
    return ((ObdParser::A(value) * 256) + ObdParser::B(value)) / 10 - 20;
}
