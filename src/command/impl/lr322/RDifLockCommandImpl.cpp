//
// Created by terranz on 06.11.26.
//

#include "RDifLockCommandImpl.h"
#include "src/command/commandIds.h"
#include "src/obd/ObdParser.h"

RDifLockCommandImpl::RDifLockCommandImpl()
    : AbstractCommand(CMD_RDIF_LOCK, "Rear differential lock command",
                      &ObdResult::rdifLock) {}

int RDifLockCommandImpl::calculate(const QString &value) const {
    // Documented as "1-{A:1}": inversion of bit 1 (0x02) of byte A.
    // NOTE: confirm the exact bit and condition against a real ECU response
    // before relying on this in production.
    return 1 - ((ObdParser::A(value) >> 1) & 1);
}
