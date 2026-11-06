//
// Created by terranz on 06.11.26.
//

#ifndef ABSFRCOMMANDIMPL_H
#define ABSFRCOMMANDIMPL_H
#include "src/command/AbstractCommand.h"


class AbsFRCommandImpl final : public AbstractCommand {
public:
    AbsFRCommandImpl();

    int calculate(const QString &value) const override;
};


#endif // ABSFRCOMMANDIMPL_H
