//
// Created by terranz on 27.02.25.
//

#ifndef TCTEMPCOMMANDIMPL_H
#define TCTEMPCOMMANDIMPL_H
#include "src/command/AbstractCommand.h"


class TCTempCommandImpl final : public AbstractCommand {
public:
    TCTempCommandImpl();

    int calculate(const QString &value) override;
};


#endif //TCTEMPCOMMANDIMPL_H
