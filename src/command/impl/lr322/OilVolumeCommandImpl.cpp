//
// Created by terranz on 27.02.25.
//

#include "OilVolumeCommandImpl.h"
#include <src/command/commandIds.h>
#include <src/obd/ObdParser.h>

OilVolumeCommandImpl::OilVolumeCommandImpl() : AbstractCommand(CMD_LVL_OIL_VOL, "Oil Volume Command", &ObdResult::lvlOilVol)
{
}

int OilVolumeCommandImpl::calculate(const QString &value) const
{
    return ObdParser::A(value) / 10;
}
