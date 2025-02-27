#include "SelectProtocolObdCommandImpl.h"

SelectProtocolObdCommandImpl::SelectProtocolObdCommandImpl() : AbstractCommand("ATSP6", "Select Protocol Command") {
}

QString SelectProtocolObdCommandImpl::calculate(const QString &value) {
    return AbstractCommand::calculate(value);
}
