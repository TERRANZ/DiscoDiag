#include <src/command/commandIds.h>
#include <src/command/ObdParser.h>
#include "TransferCaseSolenoidPositionCommandImpl.h"

TransferCaseSolenoidPositionCommandImpl::TransferCaseSolenoidPositionCommandImpl() : AbstractCommand(TC_SOL_POS,
                                                                                                     "Transfer Case Solenoid position") {}

QString TransferCaseSolenoidPositionCommandImpl::calculate(const QString &value) const {
    return (int) ObdParser::extractDigitA(value) > 0 ? "Hi" : "Lo";
}
