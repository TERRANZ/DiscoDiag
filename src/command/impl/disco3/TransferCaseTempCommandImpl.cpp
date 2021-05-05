#include <src/command/commandIds.h>
#include <src/command/ObdParser.h>
#include "TransferCaseTempCommandImpl.h"

TransferCaseTempCommandImpl::TransferCaseTempCommandImpl() : AbstractCommand(TC_TEMP, "Select Protocol Command") {
}

QString TransferCaseTempCommandImpl::calculate(const QString &value) const {
    return QString().setNum(((int) ObdParser::extractDigitA(value)) - 40);
}
