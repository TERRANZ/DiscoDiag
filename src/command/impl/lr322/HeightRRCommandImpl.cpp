//
// Created by terranz on 06.11.26.
//

#include "HeightRRCommandImpl.h"
#include "src/command/commandIds.h"
#include "src/obd/ObdParser.h"

HeightRRCommandImpl::HeightRRCommandImpl()
    : AbstractCommand(CMD_HEIGHT_RR, "Suspension height RR command",
                      &ObdResult::heightRR) {}

int HeightRRCommandImpl::calculate(const QString &value) const {
    return ((ObdParser::A(value) * 256) + ObdParser::B(value)) / 10 - 20;
}
