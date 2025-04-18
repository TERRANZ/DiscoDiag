//
// Created by terranz on 27.02.25.
//

#ifndef GBTEMPCOMMANDIMPL_H
#define GBTEMPCOMMANDIMPL_H
#include "src/command/AbstractCommand.h"


class GbTempCommandImpl final : public AbstractCommand {
public:
    GbTempCommandImpl();

    int calculate(const QString &value) override;
};


#endif //GBTEMPCOMMANDIMPL_H
