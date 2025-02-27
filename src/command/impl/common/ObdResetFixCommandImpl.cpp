#include "ObdResetFixCommandImpl.h"

ObdResetFixCommandImpl::ObdResetFixCommandImpl() : AbstractCommand("ATZ", "Obd Reset") {
}

QString ObdResetFixCommandImpl::calculate(const QString &value) {
    return AbstractCommand::calculate(value);
}
