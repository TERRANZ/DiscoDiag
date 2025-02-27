//
// Created by terranz on 21.02.25.
//

#ifndef AIRTEMPCOMMANDIMPL_H
#define AIRTEMPCOMMANDIMPL_H
#include "src/command/AbstractCommand.h"


class CoolantTempCommandImpl final : public AbstractCommand {
public:
    CoolantTempCommandImpl();

    QString calculate(const QString &value) override;
};


#endif //AIRTEMPCOMMANDIMPL_H
