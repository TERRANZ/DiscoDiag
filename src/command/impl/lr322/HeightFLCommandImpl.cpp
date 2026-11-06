//
// Created by terranz on 06.11.26.
//

#include "HeightFLCommandImpl.h"
#include "src/command/commandIds.h"
#include "src/obd/ObdParser.h"

HeightFLCommandImpl::HeightFLCommandImpl()
    : AbstractCommand(CMD_HEIGHT_FL, "Suspension height FL command",
                      &ObdResult::heightFL) {}

int HeightFLCommandImpl::calculate(const QString &value) const {
    return ((ObdParser::A(value) * 256) + ObdParser::B(value)) / 10 - 20;
}
