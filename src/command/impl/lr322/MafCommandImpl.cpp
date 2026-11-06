//
// Created by terranz on 06.11.26.
//

#include "MafCommandImpl.h"
#include "src/command/commandIds.h"
#include "src/obd/ObdParser.h"

MafCommandImpl::MafCommandImpl()
    : AbstractCommand(CMD_MAF, "Mass air flow command", &ObdResult::maf) {}

int MafCommandImpl::calculate(const QString &value) const {
    return ObdParser::A(value) * 25.6 + ObdParser::B(value) * 0.01;
}
