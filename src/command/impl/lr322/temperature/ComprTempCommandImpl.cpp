//
// Created by terranz on 27.02.25.
//

#include "ComprTempCommandImpl.h"
#include <src/command/commandIds.h>
#include <src/obd/ObdParser.h>

ComprTempCommandImpl::ComprTempCommandImpl() : AbstractCommand(CMD_TEMP_COMPR, "Compressor temp command", &ObdResult::tempCOMPR) {}

int ComprTempCommandImpl::calculate(const QString &value) const
{
    return ObdParser::A(value) - 40;
}
