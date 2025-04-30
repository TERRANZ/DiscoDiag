//
// Created by terranz on 27.02.25.
//

#include "ComprTempCommandImpl.h"
#include <src/command/commandIds.h>
#include <src/obd/ObdParser.h>

ComprTempCommandImpl::ComprTempCommandImpl() : AbstractCommand(CMD_TEMP_COMPR, "Ambient air temp command") {}

int ComprTempCommandImpl::calculate(const QString &value)
{
    return ObdParser::extractDigitA(value) - 40;
}
