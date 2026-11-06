//
// Created by terranz on 06.11.26.
//

#include "MileageCommandImpl.h"
#include "src/command/commandIds.h"
#include "src/obd/ObdParser.h"

MileageCommandImpl::MileageCommandImpl()
    : AbstractCommand(CMD_MILEAGE, "Odometer command", &ObdResult::mileage) {}

int MileageCommandImpl::calculate(const QString &value) const {
    // 24-bit value spread across three payload bytes.
    return ObdParser::A(value) * 65536 +
           ObdParser::B(value) * 256 +
           ObdParser::C(value);
}
