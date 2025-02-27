//
// Created by terranz on 21.02.25.
//

#include "coolanttempcommandimpl.h"

#include <QList>

#include "src/command/commandIds.h"

CoolantTempCommandImpl::CoolantTempCommandImpl() : AbstractCommand(CMD_TEMP_COOLANT, "Coolant temp command") {
}

QString CoolantTempCommandImpl::calculate(const QString &value) {
    auto split = value.split('\n');
    auto header = split.at(0);
    auto temp1 = split.at(1);
    auto temp2 = split.at(2);
    printf("Header: %s\n", header.toStdString().c_str());
    printf("Temp1: %s\n", temp1.toStdString().c_str());
    printf("Temp2: %s\n", temp2.toStdString().c_str());
    return "qeq2eq";
}
