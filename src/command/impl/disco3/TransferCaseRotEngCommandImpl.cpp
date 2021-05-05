#include <src/command/commandIds.h>
#include <src/command/ObdParser.h>
#include "TransferCaseRotEngCommandImpl.h"

TransferCaseRotEngCommandImpl::TransferCaseRotEngCommandImpl() : AbstractCommand(TC_ROT_ENG,
                                                                                 "Transfer case rotation") {}

QString TransferCaseRotEngCommandImpl::calculate(const QString &value) const {
    auto digA = (int) ObdParser::extractDigitA(value);
    auto digB = (int) ObdParser::extractDigitB(value);
    auto result = ((digA * 2039) / 127) + ((digB * 20) / 127);
    return QString().setNum(result);
}
