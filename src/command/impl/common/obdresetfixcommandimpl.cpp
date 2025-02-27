#include "obdresetfixcommandimpl.h"

ObdResetFixCommandImpl::ObdResetFixCommandImpl() : AbstractCommand("ATZ", "Obd Reset") {
}

QString ObdResetFixCommandImpl::calculate(const QString &value) {
    return AbstractCommand::calculate(value);
}
