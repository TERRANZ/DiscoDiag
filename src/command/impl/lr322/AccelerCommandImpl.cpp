//
// Created by terranz on 06.11.26.
//

#include "AccelerCommandImpl.h"
#include "src/command/commandIds.h"
#include "src/obd/ObdParser.h"

AccelerCommandImpl::AccelerCommandImpl()
    : AbstractCommand(CMD_ACCELER, "Longitudinal acceleration command",
                      &ObdResult::acceler) {}

int AccelerCommandImpl::calculate(const QString &value) const {
    // High byte is signed; the whole result is expressed in 1/100 g.
    return ObdParser::SIGNED(ObdParser::A(value)) * 32.8 / 255 +
           ObdParser::B(value) / 100;
}
