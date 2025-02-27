//
// Created by terranz on 25.02.25.
//

#ifndef EXTPARAMSCOMMANDIMPL_H
#define EXTPARAMSCOMMANDIMPL_H
#include "src/command/abstractcommand.h"


class ExtParamsCommandImpl final : public AbstractCommand {
public:
    ExtParamsCommandImpl();

    QString calculate(const QString &value) override;
};


#endif //EXTPARAMSCOMMANDIMPL_H
