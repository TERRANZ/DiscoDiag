//
// Created by terranz on 27.02.25.
//

#include "BrakePressCommandImpl.h"
#include "src/command/commandIds.h"
#include "src/obd/ObdParser.h"
#include <qcontainerfwd.h>

BrakePressCommandImpl::BrakePressCommandImpl()
    : AbstractCommand(CMD_PRESS_BRAKE, "Brake pressure command",
                      &ObdResult::pressBrake) {}

int BrakePressCommandImpl::calculate(const QString &value) const {
  return ObdParser::A(value) * 8533.3337 +
         ObdParser::B(value) * 33.33;
}