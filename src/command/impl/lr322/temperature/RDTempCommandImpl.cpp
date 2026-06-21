//
// Created by terranz on 27.02.25.
//

#include "RDTempCommandImpl.h"
#include "src/command/commandIds.h"
#include "src/obd/ObdParser.h"

RDTempCommandImpl::RDTempCommandImpl() : AbstractCommand(CMD_TEMP_RD, "Rear diff temp command") {
}

int RDTempCommandImpl::calculate(const QString &value) {
    return ObdParser::A(value) - 40;
}
