//
// Created by terranz on 27.02.25.
//

#ifndef AMBIENTAIRTEMPCOMMANDIMPL_H
#define AMBIENTAIRTEMPCOMMANDIMPL_H
#include "src/command/AbstractCommand.h"


class AmbientAirTempCommandImpl final : public AbstractCommand {
public:
    AmbientAirTempCommandImpl();

    QString calculate(const QString &value) override;
};


#endif //AMBIENTAIRTEMPCOMMANDIMPL_H
