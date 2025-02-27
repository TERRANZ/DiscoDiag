//
// Created by terranz on 27.02.25.
//

#ifndef INTAKEAIRTEMPCOMMANDIMPL_H
#define INTAKEAIRTEMPCOMMANDIMPL_H
#include "src/command/AbstractCommand.h"


class IntakeAirTempCommandImpl final : public AbstractCommand {
public:
    IntakeAirTempCommandImpl();

    QString calculate(const QString &value) override;
};


#endif //INTAKEAIRTEMPCOMMANDIMPL_H
