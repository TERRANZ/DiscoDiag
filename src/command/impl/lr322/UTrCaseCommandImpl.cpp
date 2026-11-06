//
// Created by terranz on 06.11.26.
//

#include "UTrCaseCommandImpl.h"
#include "src/command/commandIds.h"
#include "src/obd/ObdParser.h"

UTrCaseCommandImpl::UTrCaseCommandImpl()
    : AbstractCommand(CMD_U_TRCASE, "Transfer case temp sensor voltage command",
                      &ObdResult::uTrCase) {}

int UTrCaseCommandImpl::calculate(const QString &value) const {
    return ObdParser::A(value) / 4;
}
