//
// Created by terranz on 06.11.26.
//

#include "MainPressCommandImpl.h"
#include "src/command/commandIds.h"
#include "src/obd/ObdParser.h"

MainPressCommandImpl::MainPressCommandImpl()
    : AbstractCommand(CMD_MAIN_PRES, "Mainline pressure command",
                      &ObdResult::mainPres) {}

int MainPressCommandImpl::calculate(const QString &value) const {
    return (ObdParser::A(value) * 256) + ObdParser::B(value);
}
