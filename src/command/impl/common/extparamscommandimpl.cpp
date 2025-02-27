//
// Created by terranz on 25.02.25.
//

#include "extparamscommandimpl.h"

ExtParamsCommandImpl::ExtParamsCommandImpl() : AbstractCommand("", "Extended params command") {
}

QString ExtParamsCommandImpl::calculate(const QString &value) {
    return AbstractCommand::calculate(value);
}
