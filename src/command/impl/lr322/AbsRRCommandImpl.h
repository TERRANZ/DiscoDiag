//
// Created by terranz on 06.11.26.
//

#ifndef ABSRRCOMMANDIMPL_H
#define ABSRRCOMMANDIMPL_H
#include "src/command/AbstractCommand.h"


class AbsRRCommandImpl final : public AbstractCommand {
public:
    AbsRRCommandImpl();

    int calculate(const QString &value) const override;
};


#endif // ABSRRCOMMANDIMPL_H
